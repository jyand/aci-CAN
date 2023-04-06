#ifndef ECSCTL_H
#define ECSCTL_H
#endif

#include <stdint.h>

unsigned short ExtractTemperature(unsigned long word) ;
unsigned long SetCabinTemperature(unsigned short temperature) ;
unsigned long SetCabinFanSpeed(unsigned char speed) ;
unsigned long CabinZoneSetPoint(unsigned short temperature) ;

#define ECSCTL_CLASS 0x20U
#define TOUCHSCREEN_CLASS 0x53U

#define LABEL201 0x81UL
#define LABEL203 0x83UL
#define LABEL101 0x41UL
#define LABEL103 0x43UL
#define LABEL105 0x45UL

// all data is for Cabin, not Cockpit
struct ECSdata {
        int16_t temp_current ;
        int16_t temp_setpoint ;
        unsigned char cabinfanspeed ;
        unsigned char ecsmode ;
        uint8_t compressorstatus ;
} ;
