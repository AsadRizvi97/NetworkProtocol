#include "student_common_GBN.h"

struct pkt packetsB[101];
int baseB = 1;
int nextseqnumB = 1;
int windowB = 3;
int expectedseqnumB = 1;
struct pkt sndpktB;

/*
 * Just like A_output, but residing on the B side.  USED only when the
 * implementation is bi-directional.
 */
void B_output(struct msg message)  {
    CommonOutput(BEntity, message, &nextseqnumB, baseB, packetsB);
}

/*
 * B_input(packet),where packet is a structure of type pkt. This routine
 * will be called whenever a packet sent from the A-side (i.e., as a result
 * of a tolayer3() being done by a A-side procedure) arrives at the B-side.
 * packet is the (possibly corrupted) packet sent from the A-side.
 */
void B_input(struct pkt packet) {
    CommonInput(BEntity, packet, &baseB, nextseqnumB, &expectedseqnumB, &sndpktB);
}

/*
 * B_timerinterrupt()  This routine will be called when B's timer expires
 * (thus generating a timer interrupt). You'll probably want to use this
 * routine to control the retransmission of packets. See starttimer()
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */
void  B_timerinterrupt() {
    CommonInterrupt(BEntity, baseB, windowB, nextseqnumB, packetsB);
}

/*
 * The following routine will be called once (only) before any other
 * entity B routines are called. You can use it to do any initialization
 */
void B_init() {
    CommonInit(&sndpktB);
}
