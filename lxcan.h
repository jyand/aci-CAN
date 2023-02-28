#include "acican.h"

#ifndef _LX_CAN_H_
#define _LX_CAN_H_
#endif

#define CAN_QUEUE_LEN 30

struct CANPacket {
        unsigned char data[8] ;
        unsigned char devclass ;
        unsigned char devID ;
        unsigned char subID ;
} ;

struct CANQueue {
        struct CANPacket packet[CAN_QUEUE_LEN] ;
        unsigned char head ;
        unsigned char next ;
        unsigned char length ;
} ;

unsigned long GenCANID(const struct CANPacket *pkt) {
        const unsigned long magic = (0x5 << 25) ;
        unsigned long word = magic | (((unsigned long)pkt->devclass << 16) & (0xFFUL << 16)) ;
        word |= (((unsigned long)pkt->devID << 8) & (0xFFUL << 8)) ;
        word |= (unsigned long)pkt->subID & 0xFFUL ;
        return word ;
}

void SendCANPacket(const struct CANPacket *pkt) {
        const unsigned long timeout = 0xFFFFFFF ;
        unsigned long timer = 0 ;
        unsigned char canpagereg = CANPAGE ;
        char k = 0 ;
        char mobnum ;
        do {
                mobnum = k ;
                k = (k + 1)%2 ;
                ++timer ;
                asm("wdr ;") ;
        } while (((CANEN >> mobnum) & 0x1U) && timer < timeout)
        if (timer >= timeout) {
                return ;
        }
        CANPAGE = (mobnum << 4) ;
        CANCDMOB &= ~((1 << CONMOB1) | (1 << CONMOB0)) ;
        CANSTMOB & ~(1 << TXOK) ;
        CANCDMOB |= (1 << IDE) | (1 << RPLV) ;
        CANIDT4 |= (1 << RTRTAG) ;

        unsigned long tempID = GenCANID(pkt) ;
        SetExtID(tempID) ;
        CANCDMOB &= 0xF0U ;
        CANCDMOB |= ((unsigned char)sizeof(pkt->data) & ((1 << DLC3) | (1 << DLC2) | (1 << DLC1) | (1 << DLC0))) ;

        CANPAGE &= 0xF0 ;
        for (k = 0 ; k < ((unsigned char)sizeof(pkt->data)%9) ; k++) {
                CANMSG = pkt->data[k] ;
        }

        CANCDMOB &= ~((1 << CONMOB1) | (1 << CONMOB0)) ;
        CANCDMOB |= (1 << CONMOB0) ;

        CANPAGE = canpagereg ;
}
