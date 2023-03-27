#include <math.h>
#include "ecsctl.h"

/* create 429 word for transmission */

unsigned long SetCabinFanSpeed(unsigned char speed) {
        auto const unsigned char AUTO_SPEED = 15 ;
        auto const unsigned char FAN_SPEED_START = 14 ;
        if (speed > 10) {
                speed = AUTO_SPEED ;
        }
        //return (speed << FAN_SPEED_START) | LABEL201 ;
        return ((unsigned long)speed << FAN_SPEED_START) | LABEL201 ;
}

unsigned long CabinZoneSetPoint(unsigned char temperature) {
        float celsius = (float)temperature*5/9 + 32 ;
        unsigned char setptfield = (unsigned char)floor(celsius) ;
        /* ignore fractional temperatures for now */
        auto const unsigned char CABIN_SETPT_START = 17 ;
        return ((unsigned long)setptfield << CABIN_SETPT_START) | LABEL203 ;
}