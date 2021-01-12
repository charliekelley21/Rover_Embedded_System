#ifndef JSON_PARSE_H_
#define JSON_PARSE_H_

#include "interconnectivity_milestone.h"

// structs for json parsing

// "subscribe"
typedef struct Task1_v2_avg{
    int sequence;
    int totMsgs;
    int totReads;
    int avgRead; // in mm
} Task1_v2_avg;

typedef struct IR_avg{
    int sequence;
    int type;
    int distance;
    int numReadings;
    int curTime;
} IR_avg;

typedef struct chain_message{
    int sequence;
    int chainNum;
    int value;
} chain_message;

// yes, this is going to be global, I'm sorry
typedef struct debug_msg{
    int chainNum;   // record which board this was
    int numPubT1;   // number of times this board has tried to publish to T1
    int numPubT2;   // number of times this board has tried to publish to T2
    int numRecvT1;  // number of times this board has received from T1
    int numRecvT2;  // number of times this board has received from T2
    int firstSeqRecT1;  //First sequence number received by struct
    int firstSeqRecT2;
    int numMissT1;  // number of publications this board has missed from T1
    int numMissT2;  // number of publications this board has missed from T2
} debug_msg;

static int jsoneq(const char *json, jsmntok_t *tok, const char *s);

int encode_analytics    (char* ret, struct debug_msg pack);
int encodeTask1_v2_avg  (char* ret, struct Task1_v2_avg pack);
int encodeIR_avg        (char* ret, struct IR_avg pack);
int encode_chain        (char* ret, struct chain_message pack);

IR_avg parseIR_avg(char* JSON_STRING);
chain_message parse_chain(char* JSON_STRING);

#endif /* JSON_PARSE_H_ */
