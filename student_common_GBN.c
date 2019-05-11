#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student_common_GBN.h"

int get_checksum(struct pkt packet) {
    int checksum = 33;
    checksum += 3*packet.checksum;
    checksum += 5*packet.seqnum;
    checksum += 7*packet.acknum;
    for (int i = 0; i < strlen(packet.payload); i++) {
        checksum += (i+1)*9*packet.payload[i];
    }
    return checksum;
}

void CommonOutput(int AorB, struct msg message, int *nextseqnum, int base, struct pkt packets[]) {
    struct pkt packet = {*nextseqnum, 0, 1, ""};
    memcpy(packet.payload, message.data, MESSAGE_LENGTH);
    packet.checksum = get_checksum(packet);
    packets[*nextseqnum] = packet;
    tolayer3(AorB, packets[*nextseqnum]);
    if (base == *nextseqnum) {
        startTimer(AorB, 100);
    }
    *nextseqnum += 1;
}

void CommonInput(int AorB, struct pkt packet, int *base, int nextseqnum, int *expectedseqnum, struct pkt *sndpkt) {
    int nulls = 0;
    for (int i = 0; i < MESSAGE_LENGTH; i++) {
        if (packet.payload[i] == '\0') nulls++;
    }
    if (nulls > 18) {
        struct pkt newpkt = {packet.seqnum, packet.acknum, 1};
        memcpy(newpkt.payload, packet.payload, MESSAGE_LENGTH);
        if (get_checksum(newpkt) == packet.checksum) {
            *base = packet.acknum + 1;
            if (*base == nextseqnum) {
                stopTimer(AorB);
            } else {
                startTimer(AorB, 100);
            }
        }
    } else {
        struct pkt newpkt = {packet.seqnum, packet.acknum, 1};
        memcpy(newpkt.payload, packet.payload, MESSAGE_LENGTH);
        if (get_checksum(newpkt) == packet.checksum && packet.seqnum == *expectedseqnum) {
            struct msg message = {""};
            memcpy(message.data, packet.payload, MESSAGE_LENGTH);
            tolayer5(AorB, message);
            struct pkt toSend = {*expectedseqnum, *expectedseqnum, 1};
            toSend.checksum = get_checksum(toSend);
            *sndpkt = toSend;
            tolayer3(AorB, *sndpkt);
            *expectedseqnum += 1;
        } else {
            tolayer3(AorB, *sndpkt);
        }
    }
}

void CommonInterrupt(int AorB, int base, int window, int nextseqnum, struct pkt packets[]) {
    startTimer(AorB, 100);
    for (int i = base; i < base + window && i < 101 && i < nextseqnum; i++) {
        tolayer3(AorB, packets[i]);
    }
}

void CommonInit(struct pkt *sndpkt) {
    struct pkt sndpktInit = {0, 0, 1, ""};
    sndpktInit.checksum = get_checksum(sndpktInit);
    *sndpkt = sndpktInit;
}
