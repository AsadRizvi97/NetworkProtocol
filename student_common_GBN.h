#include "project2.h"

void CommonOutput(int AorB, struct msg message, int *nextseqnum, int base, struct pkt packets[]);
void CommonInput(int AorB, struct pkt packet, int *base, int nextseqnum, int *expectedseqnum, struct pkt *sndpkt);
void CommonInterrupt(int AorB, int base, int window, int nextseqnum, struct pkt packets[]);
void CommonInit(struct pkt *sndpkt);
