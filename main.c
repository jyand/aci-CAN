#include "acican.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define DELAY 100

void main() __attribute__((noreturn)) ;

void main() {
        asm("cli ; wdr ;") ;
        InitCAN() ;
        struct CANPacket *packet ;
        packet = (struct CANPacket*)malloc(sizeof(struct CANPacket)) ;
        packet->devclass = 0x20 ;
        packet->devID = 0x00 ;
        packet->subID = 0x81 ;
        packet->data[0] = 0x1 ;
        packet->data[1] = 0x5B ;
        packet->data[2] = 0x1 ;
        for (;;) {
                asm("wdr ;") ;
                SendCANPacket(packet) ;
                _delay_ms(DELAY) ;
                sleep_disable() ;
        }
}
