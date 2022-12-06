#include <iostream> 
#include <unistd.h>  

typedef struct radiotap_header{
    uint8_t itVersion;
    uint8_t itPad;
    uint16_t itLen;
    uint32_t itPresent; //flags
} RTHDR;