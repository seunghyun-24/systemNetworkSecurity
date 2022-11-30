#define STD_OPN 0x0001u
#define STD_WEP 0x0002u
#define STD_WPA 0x0004u
#define STD_WPA2 0x0008u

#define STD_FIELD (STD_OPN | STD_WEP | STD_WPA | STD_WPA2)

static void find_enc(unsigned char *h80211, int caplen)
/* packet parsing: Beacon & Probe response */
	/* TODO: Merge this if and the one above */
	if ((h80211[0] == 0x80 //IEEE80211_FC0_SUBTYPE_BEACON
		 || h80211[0] == 0x50) //IEEE80211_FC0_SUBTYPE_PROBE_RESP)
		&& caplen > 38)
	{
		p = h80211 + 36; // ignore hdr + fixed params

		while (p < h80211 + caplen)
		{
			type = p[0];
			length = p[1];
			if (p + 2 + length > h80211 + caplen)
			{
				/*                printf("error parsing tags! %p vs. %p (tag:
				%i, length: %i,position: %i)\n", (p+2+length), (h80211+caplen),
				type, length, (p-h80211));
				exit(1);*/
				break;
			}

			// Find WPA and RSN tags
			if ((type == 0xDD && (length >= 8)
				 && (memcmp(p + 2, "\x00\x50\xF2\x01\x01\x00", 6) == 0))
				|| (type == 0x30))
			{
				ap_cur->security &= ~(STD_WEP | ENC_WEP | STD_WPA);

				org_p = p;
				offset = 0;

				if (type == 0xDD)
				{
					// WPA defined in vendor specific tag -> WPA1 support
					ap_cur->security |= STD_WPA;
					offset = 4;
				}

				// RSN => WPA2
				if (type == 0x30)
				{
					ap_cur->security |= STD_WPA2;
					offset = 0;
				}

				if (length < (18 + offset))
				{
					p += length + 2;
					continue;
				}

				// Number of pairwise cipher suites
				if (p + 9 + offset > h80211 + caplen) break;
				numuni = p[8 + offset] + (p[9 + offset] << 8);

				// Number of Authentication Key Managament suites
				if (p + (11 + offset) + 4 * numuni > h80211 + caplen) break;
				numauth = p[(10 + offset) + 4 * numuni]
						  + (p[(11 + offset) + 4 * numuni] << 8);

				p += (10 + offset);

				if (type != 0x30)
				{
					if (p + (4 * numuni) + (2 + 4 * numauth) > h80211 + caplen)
						break;
				}
				else
				{
					if (p + (4 * numuni) + (2 + 4 * numauth) + 2
						> h80211 + caplen)
						break;
				}

				// Get the list of cipher suites
				for (i = 0; i < (size_t) numuni; i++)
				{
					switch (p[i * 4 + 3])
					{
						case 0x01:
							ap_cur->security |= ENC_WEP;
							break;
						case 0x02:
							ap_cur->security |= ENC_TKIP;
							break;
						case 0x03:
							ap_cur->security |= ENC_WRAP;
							break;
						case 0x0A:
						case 0x04:
							ap_cur->security |= ENC_CCMP;
							ap_cur->security |= STD_WPA2;
							break;
						case 0x05:
							ap_cur->security |= ENC_WEP104;
							break;
						case 0x08:
						case 0x09:
							ap_cur->security |= ENC_GCMP;
							ap_cur->security |= STD_WPA2;
							break;
						case 0x0B:
						case 0x0C:
							ap_cur->security |= ENC_GMAC;
							ap_cur->security |= STD_WPA2;
							break;
						default:
							break;
					}
				}

				p += 2 + 4 * numuni;

				// Get the AKM suites
				for (i = 0; i < numauth; i++)
				{
					switch (p[i * 4 + 3])
					{
						case 0x01:
							ap_cur->security |= AUTH_MGT;
							break;
						case 0x02:
							ap_cur->security |= AUTH_PSK;
							break;
						case 0x06:
						case 0x0d:
							ap_cur->security |= AUTH_CMAC;
							break;
						case 0x08:
							ap_cur->security |= AUTH_SAE;
							break;
						case 0x12:
							ap_cur->security |= AUTH_OWE;
							break;
						default:
							break;
					}
				}

				p = org_p + length + 2;
			}
			else if ((type == 0xDD && (length >= 8)
					  && (memcmp(p + 2, "\x00\x50\xF2\x02\x01\x01", 6) == 0)))
			{
				// QoS IE
				ap_cur->security |= STD_QOS;
				p += length + 2;
			}
			else if ((type == 0xDD && (length >= 4)
					  && (memcmp(p + 2, "\x00\x50\xF2\x04", 4) == 0)))
			{
				// WPS IE
				org_p = p;
				p += 6;
				int len = length, subtype = 0, sublen = 0;
				while (len >= 4)
				{
					subtype = (p[0] << 8) + p[1];
					sublen = (p[2] << 8) + p[3];
					if (sublen > len) break;
					switch (subtype)
					{
						case 0x104a: // WPS Version
							ap_cur->wps.version = p[4];
							break;
						case 0x1011: // Device Name
						case 0x1012: // Device Password ID
						case 0x1021: // Manufacturer
						case 0x1023: // Model
						case 0x1024: // Model Number
						case 0x103b: // Response Type
						case 0x103c: // RF Bands
						case 0x1041: // Selected Registrar
						case 0x1042: // Serial Number
							break;
						case 0x1044: // WPS State
							ap_cur->wps.state = p[4];
							break;
						case 0x1047: // UUID Enrollee
						case 0x1049: // Vendor Extension
							if (memcmp(&p[4], "\x00\x37\x2A", 3) == 0)
							{
								unsigned char * pwfa = &p[7];
								int wfa_len = ntohs(*((short *) &p[2]));
								while (wfa_len > 0)
								{
									if (*pwfa == 0)
									{ // Version2
										ap_cur->wps.version = pwfa[2];
										break;
									}
									wfa_len -= pwfa[1] + 2;
									pwfa += pwfa[1] + 2;
								}
							}
							break;
						case 0x1054: // Primary Device Type
							break;
						case 0x1057: // AP Setup Locked
							ap_cur->wps.ap_setup_locked = p[4];
							break;
						case 0x1008: // Config Methods
						case 0x1053: // Selected Registrar Config Methods
							ap_cur->wps.meth = (p[4] << 8) + p[5];
							break;
						default: // Unknown type-length-value
							break;
					}
					p += sublen + 4;
					len -= sublen + 4;
				}
				p = org_p + length + 2;
			}
			else
				p += length + 2;
		}
	}
