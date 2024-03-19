#include "mac.h"
#include "radiotap.h"
#include <string>

#pragma pack(push, 1)
typedef struct beacon_frame_ {
    uint8_t version:2;
    uint8_t type:2;
    uint8_t subtype:4;
    uint8_t flags;
    uint16_t duration;
    Mac destMAC;
    Mac srcMAC;
    Mac BSSID; //6byte
    uint16_t seq_ctl;

    uint64_t timestamp;
    uint16_t beaconInterval;
    uint16_t capabilityInfo;
} BF_;
#pragma pack(pop)

/*
typedef struct beacon_frame_fixed {
    uint64_t timestamp;
    uint16_t beaconInterval;
    uint16_t capabilityInfo;
    // 이제 이 뒤에 SSID가 variable size로 오게 됨
} BFF;
*/

//typedef struct fixedBeacon = (BFF*)( (BF*)(RTHDR) + (BF*)radiotap->itLen + sizeof(BF));

//typedef struct fixedBeaconAfter = (fixedBeacon + sizeof(BFF));

#pragma pack(push, 1)
typedef struct beacon_frame_body_SSID {
    uint8_t id;
    uint8_t len;
    char essid[32];
} BFSSID;
#pragma pack(pop)

#pragma pack(push, 1)
struct BEdata {
    int beacon;
    std::string essid;

    void ADD() {
        this->beacon++;
    }
};
#pragma pack(pop)
/*
string ESSID (const u_char *packet) {

    RTHDR* radiotap = (RTHDR*)packet;
    BF_* beacon = (BF*)(packet + radiotap->itLen);

    Mac bssid = beacon->BSSID;
    BF_* FBAfter = (beacon + sizeof(BF_));

    char* tag = ((char*)FBAfter)+sizeof(FBAfter);
    
    BFSSID ssid;
    ssid.len = *(tag+1);
    ssid.essid = string(tag+2, ssid.len);
    
    //uint8_t essidLen = *(tag+1);
    //string essid = string (tag+2, essidLen);

    return (char*)(ssid.essid);
}
*/
