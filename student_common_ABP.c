#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student_common_ABP.h"

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

void CommonOutput(int AorB, struct msg message, int expectseq, int expectack, struct pkt *packetAorB) {
    struct pkt packet = {expectseq, expectack, 0, ""};
    memcpy(packet.payload, message.data, MESSAGE_LENGTH);
    packet.checksum = get_checksum(packet);
    *packetAorB = packet;
    tolayer3(AorB, packet);
    startTimer(AorB, 200);
}

void CommonInput(int AorB, struct pkt packet, int *expectseq, int *expectack, struct pkt packetAorB) {
    int nulls = 0;
    for (int i = 0; i < MESSAGE_LENGTH; i++) {
        if (packet.payload[i] == '\0') nulls++;
    }
    if (nulls > 18) {
        //printf("\nACK: %s\n\n", packet.payload);
        struct pkt newpkt = {packet.seqnum, packet.acknum, 0};
        memcpy(newpkt.payload, packet.payload, MESSAGE_LENGTH);
        if (get_checksum(newpkt) != packet.checksum || packet.acknum != *expectack) {
            tolayer3(AorB, packetAorB);
        } else {
            stopTimer(AorB);
            *expectseq = !*expectseq;
            *expectack = !*expectack;
        }
    } else {
        //printf("PKT: %s\n\n", packet.payload);
        struct pkt newpkt = {packet.seqnum, packet.acknum, 0};
        memcpy(newpkt.payload, packet.payload, MESSAGE_LENGTH);
        if (get_checksum(newpkt) != packet.checksum) {
            struct pkt NAK = {*expectseq, !*expectack, 0};
            NAK.checksum = get_checksum(NAK);
            tolayer3(AorB, NAK);
        } else if (packet.seqnum != *expectseq) {
            struct pkt ACK = {!*expectseq, !*expectack, 0};
            ACK.checksum = get_checksum(ACK);
            tolayer3(AorB, ACK);
        } else {
            struct msg message = {""};
            //printf("Extracting message at B\n");
            memcpy(message.data, packet.payload, MESSAGE_LENGTH);
            //printf("Sending message from B to app\n");
            tolayer5(AorB, message);
            struct pkt ACK = {*expectseq, *expectack, 0};
            ACK.checksum = get_checksum(ACK);
            *expectseq = !*expectseq;
            *expectack = !*expectack;
            tolayer3(AorB, ACK);
        }
    }
}

void CommonInterrupt(int AorB, struct pkt packetAorB) {
    tolayer3(AorB, packetAorB);
    startTimer(AorB, 200);
}

void CommonInit(struct pkt *packetAorB, int *expectseq, int *expectack) {
    struct pkt initpkt = {0, 0, 0, ""};
    *packetAorB = initpkt;
    *expectseq = 0;
    *expectack = 0;
}
