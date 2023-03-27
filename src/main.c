#include <stdlib.h>
#include "acican.h"
#include "aci429.h"

//static const unsigned short CTL_REG_INITIAL = 0xE838 ;

void main() __attribute__((noreturn)) ;

void main() {
        asm("cli ; wdr ;") ;
        //Initialize() ;
        //WriteACLKDiv(1) ;
        //WriteCtlReg(CTL_REG_INITIAL) ; ;
        InitCAN() ;
        struct CANPacket *pkt ; 
        pkt = (struct CANPacket*)malloc(sizeof(struct CANPacket)) ;
        pkt->devclass = 0x20 ;
        pkt->devID = 0x0 ;
        pkt->subID = 0x81 ;
        asm("sei ;") ;
        for (;;) {
                asm("wdr ;") ;
                pkt->data[0] = 0x01 ;
                pkt->data[1] = 0x5B ;
                SendCANPacket(pkt) ;
                sleep_disable() ;
        }
}
