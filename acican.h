#ifndef _ACI_CAN_H_
#define _ACI_CAN_H_
#endif

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

const unsigned long CAN_MASK = ((0xFU << 25) | (0x1U<< 24)) ;

void ClearMOb(void) ;
void ClearAllMOb(void) ;
void InitCAN(void) ;
