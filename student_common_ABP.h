#include "project2.h"

void CommonOutput(int AorB, struct msg message, int expectseq, int expectack, struct pkt *packetAorB);
void CommonInput(int AorB, struct pkt packet, int *expectseq, int *expectack, struct pkt packetAorB);
void CommonInterrupt(int AorB, struct pkt packetAorB);
void CommonInit(struct pkt *packetAorB, int *expectseq, int *expectack);
