#include <stdlib.h>
#include "acican.h"
#include "aci429.h"
#include "ecsctl.h"

static const unsigned short CTL_REG_INITIAL = 0xE038 ;

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
                asm("cli ; wdr ; sei;") ;
                for (int k = 0 ; k < 16 ; ++k) {
                        TxQueue[k] = SetCabinFanSpeed(k) ;
                        TxQueue[k + 16] = CabinZoneSetPoint(k + 60) ;
                }
                WriteTxFIFO(sizeof(TxQueue)) ;
                unsigned long temp = ReadRxFIFO() ;
                unsigned char lbl = ExtractLabel(temp) ;
                pkt->data[0] = lbl ;
                SendCANPacket(pkt) ;
                sleep_disable() ;
        }
}
