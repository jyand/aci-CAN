#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "acican.h"

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
        packet->data[] = 0x15B01 ;
        for (;;) {
                asm("wdr ;") ;
                SendCANPacket(packet) ;
                _delay_ms(DELAY) ;
                sleep_disable() ;
        }
}
