#include <math.h>
#include "ecsctl.h"

#define CABIN_ZONE_START 14

unsigned short ExtractTemperature(unsigned long word) {
        return (word >> CABIN_ZONE_START) & 0xFFFF ;
}

unsigned long SetCabinTemperature(unsigned short temperature) {
        return ((unsigned long)temperature << CABIN_ZONE_START) | LABEL203 ;
}

/* create 429 word for transmission */

unsigned long SetCabinFanSpeed(unsigned char speed) {
        const unsigned char AUTO_SPEED = 15 ;
        const unsigned char FAN_SPEED_START = 14 ;
        if (speed > 10) {
                speed = AUTO_SPEED ;
        }
        return ((unsigned long)speed << FAN_SPEED_START) | LABEL201 ;
}

unsigned long CabinZoneSetPoint(unsigned short temperature) {
        //float celsius = (float)temperature*5/9 + 32 ;
        //unsigned char setptfield = (unsigned char)celsius ;
        /* ignore fractional temperatures for now */
        const unsigned char CABIN_SETPT_START = 17 ;
        return ((unsigned long)temperature << CABIN_SETPT_START) | LABEL203 ;
}
