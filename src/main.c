#include <stdlib.h>
#include "acican.h"
#include "aci429.h"
#include "ecsctl.h"

static const unsigned short CTL_REG_INITIAL = 0xE838 ;

void main() __attribute__((noreturn)) ;

void main() {
        asm("cli ; wdr ;") ;
        Initialize() ;
        WriteACLKDiv(1) ;
        WriteCtlReg(CTL_REG_INITIAL) ; ;
        InitCAN() ;
        struct CANPacket *pkt ; 
        pkt = (struct CANPacket*)malloc(sizeof(struct CANPacket)) ;
        //g_rxpkt = (struct CANPacket*)malloc(sizeof(struct CANPacket)) ;
        pkt->devclass = 0x20 ;
        pkt->devID = 0x0 ;
        pkt->subID = 0x81 ;
        for (;;) {
                asm("wdr ;") ;
                unsigned long enviro = ReadRxFIFO() ;
                pkt->data[0] = (char)(enviro >> 24) ;
                pkt->data[1] = (char)((enviro >> 16) & 0xFF) ;
                pkt->data[2] = (char)((enviro >> 8) & 0xFF) ;
                pkt->data[3] = (char)(enviro & 0xFF) ;
                SendCANPacket(pkt) ;
                sleep_disable() ;
        }
}
