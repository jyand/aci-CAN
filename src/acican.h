#ifndef _ACI_CAN_H_
#define _ACI_CAN_H_
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <stdbool.h>
#include <stddef.h>

#define CAN_MAGIC 0xB000000

#define CAN_BAUDRATE 250000

#if CAN_BAUDRATE == 250000
#define CAN_BIT_TIMING1 0x0
#define CAN_BIT_TIMING2 0xC
#define CAN_BIT_TIMING3 0x36
#elif CAN_BAUDRATE == 125000
#define CAN_BIT_TIMING1 0x6
#define CAN_BIT_TIMING2 0x4
#define CAN_BIT_TIMING3 0x13
#endif

#define NB_MOB 6

#define TOUCHSCREEN_ID 0x53
#define ECSCTL_ID 0x20

#define CAN_QUEUE_LEN 30

struct CANPacket {
        unsigned char data[8] ;
        unsigned char devclass ;
        unsigned char devID ;
        unsigned char subID ;
} ;

struct CANQueue {
        struct CANPacket packet[CAN_QUEUE_LEN]  ;
        unsigned char first ; 
        unsigned char next ;
        unsigned char length ;
} ;

struct CANPacket *g_rxpkt ;
struct CANQueue g_canq ;

void ClearMOb(void) ;
void ClearAllMOb(void) ;
static void GetStdID(unsigned long id) ;
static void GetExtdID(unsigned long id) ;
static void SetExtdID(unsigned long id) ;
static void SetStdID(unsigned long id) ;
void InitRXMOb(unsigned char mob, unsigned long id, unsigned long mask) ;
static unsigned long GenCANID(const struct CANPacket *pkt, const bool iscmd) ;
void SendCANPacket(const struct CANPacket *pkt, const bool iscmd) ;
void GetCANPacket(struct CANPacket *pkt) ;
void InitCAN(void) ;
void SendTemperature(unsigned char subid, unsigned short temperature) ;
