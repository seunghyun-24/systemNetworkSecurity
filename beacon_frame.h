
#include <iostream>
#include "mac.h"


typedef struct radiotap_header{
    uint8_t itVersion;
    uint8_t itPad;
    uint16_t itLen;
    uint32_t itPresent; //flags
} RTHDR;

typedef RTHDR* RTHDR;

typedef struct beacon_frame {

    uint8_t version:2;
    uint8_t type:2;
	uint8_t subtype:4;
    uint8_t flags;
    uint16_t duration;
    Mac destMAC;
    Mac srcMAC;
    Mac BSSID; //6byte
    uint16_t fragNum:4;
    uint16_t seqNum:12;

} BF;

typedef BEACON* BEACON;

typedef struct beacon_frame_fixed {
    uint64_t timestamp;
    uint16_t beaconInterval;
    uint16_t capabilityInfo;
    // 이제 이 뒤에 SSID가 variable size로 오게 됨
} BFF;

typedef BFF* BFF;
