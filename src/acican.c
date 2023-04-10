#include "acican.h"
#include <avr/eeprom.h>

void SendTemperature(unsigned char subid, unsigned short temperature) {
        struct CANPacket pkt ;
        pkt.data[0] = 1 ;
        pkt.data[1] = (signed char)(temperature & 0xFFU) ;
        pkt.data[2] = (signed char)((temperature >> 8) & 0xFFU) ;
        pkt.subID = subid ;
        SendCANPacket(&pkt, true) ;
}

 void ClearMOb() {
        for (unsigned char volatile *k = &CANSTMOB ; k < &CANSTML ; ++k) {
                *k = 0x00 ;
        }
}

void ClearAllMOb() {
        for (unsigned char mobnum = 0 ; mobnum < NB_MOB ; mobnum++) {
                CANPAGE = (mobnum << 4) ;
                ClearMOb() ;
        }
}

inline static void SetExtdID(unsigned long id) {
        CANIDT1 = ((*((unsigned char*)(&(id)) + 3)) << 3) + ((*((unsigned char*)((&(id)) + 2))) >> 5) ;
        CANIDT2 = ((*((unsigned char*)(&(id)) + 2)) << 3) + ((*((unsigned char*)((&(id)) + 1))) >> 5) ;
        CANIDT3 = ((*((unsigned char*)(&(id)) + 1)) << 3) + ((*(unsigned char*)((&(id)))) >> 5) ;
        CANIDT4 = (*((unsigned char*)(&(id))) << 3) ;
}

static void GetExtdID(unsigned long id) {
        *((unsigned char*)(&(id)) + 3) = CANIDT1 >> 3 ;
        *((unsigned char*)(&(id)) + 2) = (CANIDT2 >> 3) + (CANIDT1 << 5) ;
        *((unsigned char*)(&(id)) + 1) = (CANIDT3 >> 3) + (CANIDT2 << 5) ;
        *((unsigned char*)(&(id))) = (CANIDT4 >> 3) + (CANIDT3 << 5) ;
}

static void SetStdID(unsigned long id) {
        CANIDT1 = (unsigned char)(((unsigned short)id) >> 3) ;
        CANIDT2 = (unsigned char)(((unsigned short)id) << 5) ;
        CANCDMOB &= ~(1 << IDE) ;
}

static void GetStdID(unsigned long id) {
        *((unsigned char*)(&(id)) + 1) = CANIDT1 >> 5 ;
        *((unsigned char*)(&(id))) = (CANIDT2 >> 5) + (CANIDT1 << 3) ;
}

// skip Status, Unicast, and Broadcast MOb config for now
void InitRXMOb(unsigned char mob, unsigned long id, unsigned long mask) {
        CANPAGE = (mob << 4) ;

        CANCDMOB &= ~(1 << IDE) ;
        CANCDMOB &= ~(1 << RPLV) ;
        CANIDT4 &= ~(1 << RTRTAG) ;

        SetExtdID(id) ;
        CANCDMOB |= (1 << IDE) ; 
        SetExtdID(mask) ;
        CANIDM4 |= (1 << RTRMSK) | (1 << IDEMSK) ;

        CANCDMOB &= ~((1 << DLC3) | (1 << DLC3) | (1 << DLC1) | (1 << DLC0)) ;

        CANCDMOB &= ~((1 << CONMOB1) | (1 << CONMOB0)) ;
        CANCDMOB |= (2U << CONMOB0) ;
}

void InitCAN() {
        CANGCON = (1 << SWRES) ;
        CANBT1 = CAN_BIT_TIMING1 ;
        CANBT2 = CAN_BIT_TIMING2 ;
        CANBT3 = CAN_BIT_TIMING3 ;

        CANTCON = 0x0 ;

        ClearAllMOb() ;

        for (signed char k = 2 ; k <= 4 ; ++k) {
                InitRXMOb(k, 0, ((0x1UL << 24) | (0xFUL << 25))) ;
        }
        InitRXMOb(4, 0, (0xFFUL << 16) | ((0x1UL << 24) | (0xFUL << 25)) | (0xFFUL << 8)) ;
        InitRXMOb(5, 0, (0xFFUL << 16) | ((0x1UL << 24) | (0xFUL << 25)) | (0xFFUL << 8)) ;

        CANEN2 = 0x3CU ;
        CANEN1 = 0x00U ;
        CANIE2 = 0x3CU ;
        CANIE1 = 0x00U ;
        CANGIE = 0xEA ;

        CANGCON |= (1 << ENASTB) ;
}

unsigned long GenCANID(const struct CANPacket *pkt, const bool iscmd) {
        unsigned long word ;
        if (iscmd) {
                word = 0xA000000 ;
        } else {
                word = 0xB000000 ;
        }
        word |= (((long)pkt->devclass << 16) & (0xFFUL << 16)) ;
        word |= (((long)pkt->devID << 8) & (0xFFUL << 8)) ;
        word |= (long)pkt->subID & 0xFFUL ;
        return word ;
}

void SendCANPacket(const struct CANPacket *pkt, const bool iscmd) {
        static const unsigned long timeout = 0xFFFFFFF ;
        unsigned long timer = 0 ;
        unsigned char canpagereg = CANPAGE ;
        char k = 0 ;
        char mobnum ;
        do {
                mobnum = k ;
                k = (k + 1)%2 ;
                ++timer ;
                asm("wdr ;") ;
        } while (((CANEN2 >> mobnum) & 0x1U) && timer < timeout) ;
        if (timer >= timeout) {
                return ;
        }
        CANPAGE = (mobnum << 4) ;
        CANCDMOB &= ~((1 << CONMOB1) | (1 << CONMOB0)) ;
        CANSTMOB &= ~(1 << TXOK) ;
        CANCDMOB |= (1 << IDE) | (1 << RPLV) ;
        CANIDT4 |= (1 << RTRTAG) ;

        unsigned long tempID = GenCANID(pkt, iscmd) ;
        eeprom_write_dword((void*)0xA0, tempID) ;
        SetExtdID(tempID) ;
        CANCDMOB |= (1 << IDE) ;

        CANCDMOB &= 0xF0U ;
        CANCDMOB |= ((unsigned char)sizeof(pkt->data) & ((1 << DLC3) | (1 << DLC2) | (1 << DLC1) | (1 << DLC0))) ;

        CANPAGE &= 0xF0 ;
        for (int8_t k = 0 ; k < ((unsigned char)sizeof(pkt->data)%9) ; k++) {
                CANMSG = pkt->data[k] ;
        }

        CANCDMOB &= ~((1 << CONMOB1) | (1 << CONMOB0)) ;
        CANCDMOB |= (1 << CONMOB0) ;

        CANPAGE = canpagereg ;
}

void GetCANPacket(struct CANPacket *pkt) {
        //if (CANSTMOB & (1 << RXOK)) {
                unsigned char mobnum = 2 ;
                CANPAGE = (mobnum << 4) ;

                unsigned long canid = 0 ;
                GetExtdID(canid) ;
                pkt->devclass = (canid & (0xFFUL << 16)) >> 16 ;
                pkt->devID = (canid & (0xFFUL << 8)) >> 8 ;
                pkt->subID = canid & 0xFFUL ;
                for (unsigned char index = 0 ; index < 8 ; ++index) {
                        *(pkt->data + index) = CANMSG ;
                }

                CANCDMOB &= ~((1 << CONMOB1) | (1 << CONMOB0)) ;
                CANCDMOB |= (2 << CONMOB0) ;
        //}
        CANSTMOB &= ~(1 << RXOK) ;
}
