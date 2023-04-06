#include <stdlib.h>
#include "acican.h"
#include "aci429.h"
#include "ecsctl.h"
#include <util/delay.h> 

static const unsigned short CTL_REG_INITIAL = 0xE038 ;

void main() __attribute__((noreturn)) ;

void main() {
        asm("cli ; wdr ;") ;
        InitDevice() ;
        WriteACLKDiv(1) ;
        WriteCtlReg(CTL_REG_INITIAL) ; ;
        InitCAN() ;
        struct CANPacket *pkt ; 
        pkt = (struct CANPacket*)malloc(sizeof(struct CANPacket)) ;
        g_rxpkt = (struct CANPacket*)malloc(sizeof(struct CANPacket)) ;
        pkt->devclass = 0x20 ;
        pkt->devID = 0x00 ;
        pkt->subID = 0x81 ;
        pkt->data[0] = 0x01 ;
        pkt->data[1] = 0x5B ;
        pkt->data[2] = 0xBE ;
        pkt->data[3] = 0xEF ;
        for (;;) {
                asm("wdr ;") ;
                /*unsigned long enviro = ReadRxFIFO() ;
                if (ExtractLabel(enviro) == 0x45) {
                        pkt->data[0] = 0x01 ;
                        unsigned short temp = ExtractTemperature(enviro) ;
                        eeprom_write_word((uint16_t*)0x20, temp) ;
                        eeprom_write_byte((uint8_t*)0x30, ExtractLabel(enviro)) ;
                        pkt->data[1] = (char)(temp >> 8) ;
                        pkt->data[2] = (char)(temp & 0xFF) ;
                } else if (ExtractLabel(enviro) == 0x43) {
                        pkt->data[0] = 0x0 ;
                        unsigned short temp = ExtractTemperature(enviro) ;
                        eeprom_write_word((uint16_t*)0x40, temp) ;
                        eeprom_write_byte((uint8_t*)0x50, ExtractLabel(enviro)) ;
                        pkt->data[1] = (char)(temp >> 8) ;
                        pkt->data[2] = (char)(temp & 0xFF) ;
                }
                GetCANPacket(g_rxpkt) ;
                if (g_rxpkt->devclass == 0x53) {
                        TxQueue[0] = CabinZoneSetPoint(g_rxpkt->data[0]) ;
                        TxQueue[1] = CabinZoneSetPoint(g_rxpkt->data[1]) ;
                        TxQueue[2] = CabinZoneSetPoint(g_rxpkt->data[2]) ;
                }*/
/*                for (int k = 0 ; k < sizeof(pkt->data) ; ++k) {
                        TxQueue[k] = CabinZoneSetPoint(pkt->data[k]) ;
                        //TxQueue[k + 8] = SetCabinFanSpeed(pkt->data[k]) ;
                }*/
                //for (int k = 0 ; k < 30 ; ++k) {
                        //TxQueue[k] = SetCabinFanSpeed(0xF) ;
                //}
                for (int k = 0 ; k < 16 ; ++k) {
                        TxQueue[k] = SetCabinFanSpeed(0x1) ;
                }
                _delay_ms(10000) ;
                for (int k = 0 ; k < 16 ; ++k) {
                        TxQueue[k] = SetCabinFanSpeed(0x9) ;
                }
                //for (int k = 16 ; k < 32 ; ++k) {
                        //TxQueue[k] = SetCabinTemperature(0x01FF) ;
                //}
                WriteTxFIFO(sizeof(TxQueue)) ;
                SendCANPacket(pkt, true) ;
                //SendTemperature(0x81, 0x015B) ;
                sleep_disable() ;
        }
}
