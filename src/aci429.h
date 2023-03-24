#ifndef ACI429_H
#define ACI429_H
#endif

#include <stdint.h>

void SPIWait(void) ;
void SoftMasterReset(void) ;
void WriteACLKDiv(unsigned char divisor) ;
unsigned char ReadStatusReg(void) ;
void WriteCtlReg(unsigned short halfword) ;
unsigned long ReadRxFIFO(void) ;
unsigned char ExtractLabel(unsigned long word) ;
unsigned char ExtractSDI(unsigned long word) ;
unsigned long ExtractData(unsigned long word) ;
uint8_t CheckParityBit(unsigned long word) ;

#define MASTER_RESET 0x01
#define RESET_ALL_LABELS 0x02
#define SET_ALL_LABELS  0x03
#define RESET_LABEL 0x04
#define SET_LABEL 0x05
#define LABEL_PATTERN 0x06
#define WRITE_ACLK_DIV 0x07
#define RX_DEQUEUE 0x08
#define READ_STATUSREG 0x0A
#define READ_CTLREG 0x0B
#define READ_ACLK_DIV 0x0C
#define READ_LABELTABLE 0x0D
#define TX_ENQUEUE 0x0E
#define WRITE_CTLREG 0x10
#define RESET_TX_FIFO 0x11
#define TX_EN 0x12

struct HI3585_t  {
        unsigned char clkdiv ;
        unsigned char statusreg ;
        unsigned short ctlreg ;
} ;

struct Arinc429Word {
        unsigned char label: 8 ;
        unsigned char sdi: 2 ;
        unsigned long data: 21 ;
        unsigned char parity: 1 ;
} ;
