#ifndef ECSCTL_H
#define ECSCTL_H
#endif

#include <stdint.h>

unsigned long SetCabinFanSpeed(unsigned char speed) ;
unsigned long CabinZoneSetPoint(unsigned char temperature) ;

/* for some reason 201 is the octal representation
but 203 is the decimal representation
TODO: check whether this is specific to DT-250 (ask Enviro Systems?)
*/
#define LABEL201 0x81UL
#define LABEL203 0xC1UL
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
