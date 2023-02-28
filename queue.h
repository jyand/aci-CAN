/* list-based definition in of a queue of CAN packets
not to be confused with a node in a CAN network */
struct CQ_Node {
        struct CANPacket *packet ;
        struct CQ_Node *next ; 
} ;

struct CQ_Node *head ;
struct CQ_Node *tail ;

void Enqueue(struct CANPacket *pkt) {
        struct CQ_Node *node ;
        node = (struct CQ_Node*) malloc(sizeof(struct CQ_Node)) ;
        node->packet = pkt ;
        //node->packet = (struct CANPacket*) malloc(sizeof(struct CANPacket)) ;
        node->next = NULL ;
        if ((head == NULL) && (tail == NULL)) {
                head = node ;
                tail = node ;
        } else {
                tail->next = node ;
                tail = node ;
        }
}

struct CANPacket* Dequeue() {
        if (head != NULL) {
                struct CQ_Node *temp = head ;
                struct CANPacket *pkt ;
                pkt = (struct CANPacket*) malloc(sizeof(struct CANPacket)) ;
                memcpy(pkt, head->packet, sizeof(head->packet)) ;
                //struct CANPacket *pkt = head->packet ;
                head = head->next ;
                free(temp) ;
                return pkt ;
        } else {
                return NULL ;
        }
}

int QLen() {
        static int len = 0 ;
        if (head != NULL) {
                struct CQ_Node temp = head ;
                do {
                        ++len ;
                        temp = temp->next ;
                } while (temp != NULL) ;
        }
        return len ;
}
