#include "acican.h"
#include "lxcan.h"
#include "avr/io.h"

inline void ClearMOb() {
        unsigned char *k ;
        for (k = &CANSTMOB ; k < &CANSTML ; k++) {
                *k = 0x00 ;
        }
}

inline void ClearAllMOb() {
        unsigned char mobnum ;
        for (mobnum = 0 ; mobnum < NB_MOB ; mobnum++) {
                CANPAGE = (mobnum << 4) ;
                ClearMOb() ;
        }
}

inline void SetExtID(unsigned long id) {
        CANIDT1 = (*((unsigned char*)(&(id) + 3)) << 3) + (*((unsigned char*)((&(id)) + 2)) << 5) ;
        CANIDT2 = (*((unsigned char*)(&(id) + 2)) << 3) + (*((unsigned char*)((&(id)) + 1)) << 5) ;
        CANIDT3 = (*((unsigned char*)(&(id) + 1)) << 3) + (*(unsigned char*)((&(id))) << 5) ;
        CANIDT4 = (*(unsigned char*)(&(id)) << 3) ;
}

// skip Status, Unicast, and Broadcast MOb config for now
inline void InitRXMOb(unsigned char mob, unsigned long id, unsigned long mask) {
        CANPAGE = (mob << 4) ;

        CANCDMOB &= ~(1 << IDE) ;
        CANCDMOB &= ~(1 << RPLV) ;
        CANIDT4 &= ~(1 << RTRTAG) ;

        SetExtID(id) ;
        CANCDMOB |= (1 << IDE) ; 
        SetExtID(mask) ;
        CANIDM4 |= (1 << RTRMSK) | (1 << IDEMSK) ;

        CANCDMOB &= ~((1 << DLC3) | (1 << DLC3) | (1 << DLC1) | (1 << DLC0)) ;

        CANCDMOB &= ~((1 << CONMOB1) | (1 << CONMOB0)) ;
        CANCDMOB |= (2U << CONMOB) ;
}

void InitCAN() {
        CANGCON = (1 << SWRES) ;
        CANBT1 = CAN_BIT_TIMING1 ;
        CANBT2 = CAN_BIT_TIMING2 ;
        CANBT3 = CAN_BIT_TIMING3 ;

        CANTCON = 0xFF ;

        ClearAllMOb() ;

        CANEN2 = 0x3CU ;
        CANEN1 = 0x00U ;
        CANIE2 = 0x3CU ;
        CANIE1 = 0x00U ;
        CANGIE = 0xEA ;

        CANGCON |= (1 << ENASTB) ;
}
