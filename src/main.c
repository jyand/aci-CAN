#include <stdlib.h>
#include "acican.h"

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
        packet->data[4] = 0xDE ;
        packet->data[5] = 0xAD ;
        packet->data[6] = 0xBE ;
        packet->data[7] = 0xEF ;
        asm("sei ;") ;
        for (;;) {
                asm("wdr ;") ;
                SendCANPacket(packet) ;
                sleep_disable() ;
        }
}
