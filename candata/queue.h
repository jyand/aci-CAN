#ifndef _QUEUE_H_
#define _QUEUE_H_
#endif

#include "acican.h"

/* list-based definition in of a queue of CAN packets
not to be confused with a node in a CAN network */
struct CQ_Node {
        struct CANPacket *packet ;
        struct CQ_Node *next ; 
} ;

void Enqueue(struct CANPacket *pkt) ;
struct CANPacket* Dequeue(void) ;
int QLen(void) ;
