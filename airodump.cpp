
// pcap을 열고

//bssid는 mac header에서 2 2 6 6 만큼 지난 곳에 6바이트 읽어들이면 될 것 같다
// 끝에서 2바이트 안쪽

//ssid는 맥헤더 뒤에 8 2 2 읽고 나면 그 뒤에 있는 듯..? -> 근데 이게 가변크기라는 것 같음!

//안테나 시그널이 pwr 

//ds parameter set
//enc는 wpa2

#include "beacon_frame.h"
#include "mac.h"
#include <pacp.h>
#include <iostream>
#include <map>

using namespace std;

void print_info(map<Mac,pair<uint64_t,string>>& info)
{
    puts("-------------------");
    printf("BSSID\t\t    BEACONS\tESSID\n");
    auto iter = info.begin();
    while(iter!=info.end())
    {
        cout << string(iter->first) << '\t' << (iter->second).first << '\t' << (iter->second).second << endl;
        ++iter;
    }
    puts("-------------------");
}

void airodump(const u_char *packet){

    map<Mac, pair<uint64_t, string>> info;

    RTHDR* radiotap = (RTHDR*)packet;

    //uint8_t pwr = packet + (radiotap->itLen - 4);
//#ifdef DEBUG
//    uint8_t pwr2 = packet + (radiotap->itLen - 12);
//    if (pwr != pwr2) {
//        printf("pwr is error?\n");
//        return;
//    }
//#endif


    BF* beacon = (BF*)(packet + radiotap->itLen);

    if(beacon->subtype != 8) continue; 

    Mac bssid = beacon->BSSID;
    BFF* fixedBeacon = (BFF*)(beacon + sizeof(BF));
    BFF* FBAfter = (fixedBeacon + sizeof(BFF));

    char* tag = ((char*)FBAfter)+sizeof(FBAfter);
    uint8_t = essidLen = *(tag+1);
    string essid = string (tag+2, essidLen);

    auto iter = info.find(bssid);

    if(iter == info.end()){
        pair<uint64_t, sting> tmp;
        tmp = {1, essid};
        info[bssid] = tmp;
    }
    else ++(iter->second).first;

    print_info(info);   
};

int main(int argc, char* argv[]){

    if(argc != 2){
        printf("syntax : airodump <interface> \n");
        printf("sample : \n");
        return -1;
    }

    char *data = argv[1];
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_live(dev, BUFSIZ, 1, 100, errbuf);

    if (handle == NULL)
    {
        fprintf(stderr, "cannot open device %s(%s)\n", data, errbuf);
        return -1;
    }
    
    struct pcap_pkthdr* header;
    const u_char *packet;
    
    while(true){
        int res = pcap_next_ex(handle, &header, &packet);

        if(res == 0) continue;
        if(res == -1 || res == -2){
            printf("pcap_next_ex error\n");
            break;
        }

        airodump(packet);
    }
    
    pcap_close(handle);
    return 0;
}

