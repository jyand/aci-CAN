#include "aci429.h"
#include "lx64m1.h"

inline void SPIWait() {
        while (!(SPSR & (1 << SPIF))) {}
}

void SoftMasterReset() {
        unsigned char dum = SPSR ;
        PORTC &= ~(1 << SELECT) ;
        SPDR = MASTER_RESET ;
        SPIWait() ;
        dum = SPDR ;
        PORTC |= (1 << SELECT) ;
}

void WriteACLKDiv(unsigned char divisor) {
        PORTC &= ~(1 << SELECT) ;
        unsigned char dum = SPSR ;
        SPDR = WRITE_ACLK_DIV ;
        SPIWait() ;
        dum = SPDR ;
        SPDR = divisor ;
        SPIWait() ;
        dum = SPDR ;
        PORTC |= (1 << SELECT) ;
}

unsigned char ReadStatusReg() {
        unsigned char byte = SPSR ;
        PORTC &= ~(1 << SELECT) ;
        SPDR = READ_STATUSREG ;
        SPIWait() ;
        byte = SPDR ;
        PORTC |= (1 << SELECT) ;
        return byte ;
}

void WriteCtlReg(unsigned short halfword) {
        PORTC &= ~(1 << SELECT) ;
        unsigned char dum = SPSR ;
        SPDR = WRITE_CTLREG ;
        SPIWait() ;
        dum = SPDR ;
        SPDR = (char)(halfword >> 8) ;
        SPIWait() ;
        dum = SPDR ;
        SPDR = (char)(halfword & 0xFF) ;
        SPIWait() ;
        dum = SPDR ;
        PORTC |= (1 << SELECT) ;
}

unsigned long ReadRxFIFO() {
        unsigned long word = SPSR ;
        PORTC &= ~(1 << SELECT) ;
        SPDR = RX_DEQUEUE ;
        SPIWait() ;
        SPDR = 0 ;
        SPIWait() ;
        word = (long)SPDR << 24 ;
        SPDR = 0 ;
        SPIWait() ;
        word = (long)SPDR << 16 ;
        SPDR = 0 ;
        SPIWait() ;
        word = (long)SPDR << 8 ;
        SPDR = 0 ;
        SPIWait() ;
        word |= (long)SPDR ;
        PORTC |= (1 << SELECT) ;
        return word ;
}

unsigned char ExtractLabel(unsigned long word) {
        return word & 0xFF ;
}

unsigned char ExtractSDI(unsigned long word) {
        auto const unsigned char SDI_FIELD_START = 8 ;
        auto const unsigned long SDI_FIELD_MASK = 0xF ;
        return (word >> SDI_FIELD_START) & SDI_FIELD_MASK ;
}

unsigned long ExtractData(unsigned long word) {
        auto const unsigned char DATA_FIELD_START = 10 ;
        auto const unsigned char DATA_FIELD_MASK = 0x7FFFF ;
        return (word >> DATA_FIELD_START) & DATA_FIELD_MASK ;
}

uint8_t CheckParityBit(unsigned long word) {
        if ((int32_t)word < 0) {
                return 1 ;
        } else {
                return 0 ;
        }
}