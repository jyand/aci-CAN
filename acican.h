#ifndef _ACI_CAN_H_
#define _ACI_CAN_H_
#endif

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define CAN_BAUDRATE 250

#if CAN_BAUDRATE == 500
#define CAN_BIT_TIMING1 0x06
#define CAN_BIT_TIMING2 0x04
#define CAN_BIT_TIMING3 0x13
#elif CAN_BAUDRATE == 250
#define CAN_BIT_TIMING1 0x06
#define CAN_BIT_TIMING2 0x0C
#define CAN_BIT_TIMING3 0x37
#else
#define CAN_BIT_TIMING1 0x00
#define CAN_BIT_TIMING2 0x00
#define CAN_BIT_TIMING3 0x00
#endif

#define NB_MOB 6

#define CAN_MASK = ((0xFU << 25) | (0x1U<< 24)) ;
#define CAN_QUEUE_LEN 30

struct CANPacket {
        unsigned char data[8] ;
        unsigned char devclass ;
        unsigned char devID ;
        unsigned char subID ;
} ;

void ClearMOb(void) ;
void ClearAllMOb(void) ;
void SetExtID(unsigned long id) ;
void InitRXMOb(unsigned char mob, unsigned long id, unsigned long mask) ;
unsigned long GenCANID(const struct CANPacket *pkt) ;
void SendCANPacket(const struct CANPacket *pkt) ;
void InitCAN(void) ;
