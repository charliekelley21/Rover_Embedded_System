#include "json_parser.h"

static int jsoneq(const char *json, jsmntok_t *tok, const char *s)
{
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

int encode_analytics(char* ret, struct debug_msg pack)
{
    return snprintf(ret, PAYLOAD_SIZE, "{\"chainNum\": %d, \"numPubT1\": %d, \"numPubT2\": %d, \"numRecvT1\": %d, \"numRecvT2\": %d, \"numMissT1\": %d, \"numMissT2\": %d, \"data\": 999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999}", pack.chainNum, pack.numPubT1, pack.numPubT2, pack.numRecvT1, pack.numRecvT2, pack.numMissT1, pack.numMissT2);
}

int encodeTask1_v2_avg(char* ret, struct Task1_v2_avg pack)
{
    return snprintf(ret, PAYLOAD_SIZE, "{\"sequence\": %d, \"totMsgs\": %d, \"totReads\": %d, \"avgRead\": %d}", pack.sequence, pack.totMsgs, pack.totReads, pack.avgRead);
}

int encodeIR_avg(char* ret, struct IR_avg pack)
{
    return snprintf(ret, PAYLOAD_SIZE, "{\"sequence\": %d, \"type\": %d, \"distance\": %d, \"numReadings\": %d, \"curTime\": %d}", pack.sequence, pack.type, pack.distance, pack.numReadings, pack.curTime);
}

int encode_chain(char* ret, struct chain_message pack)
{
    return snprintf(ret, PAYLOAD_SIZE, "{\"sequence\": %d, \"chainNum\": %d, \"value\": %d}", pack.sequence, pack.chainNum, pack.value);
}

IR_avg parseIR_avg(char* JSON_STRING)
{
    // terminate at '}'
    int i;
    for (i = 0; i < strlen(JSON_STRING)-1; i ++)
    {
        if (JSON_STRING[i] == '}')
        {
            *(JSON_STRING + i+1) = '\0';
            break;
        }
    }

    IR_avg package;
    int r;
    jsmn_parser p;
    jsmntok_t t[64]; /* We expect no more than 128 tokens */

    jsmn_init(&p);
    r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t) / sizeof(t[0]));

    if (r < 0)
    {
        package.sequence = -1;
//        dbgErrorRoutine(ERROR_JSMN_PARSE);
    }

    /* Assume the top-level element is an object */
    if (r < 1 || t[0].type != JSMN_OBJECT)
    {
        package.sequence = -1;
//        dbgErrorRoutine(ERROR_JSMN_PARSE);
    }

    /* Loop over all keys of the root object */
    for (i = 1; i < r; i++) {

        char type_str[10];
        if (jsoneq(JSON_STRING, &t[i], "sequence") == 0) {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            package.sequence = atoi(type_str);
            i++;
        }
        else if (jsoneq(JSON_STRING, &t[i], "type") == 0) {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            package.type = atoi(type_str);
            i++;
        }
        else if (jsoneq(JSON_STRING, &t[i], "distance") == 0) {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            package.distance = atoi(type_str);
            i++;
        }
        else if (jsoneq(JSON_STRING, &t[i], "numReadings") == 0) {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            package.numReadings = atoi(type_str);
            i++;
        }
        else if (jsoneq(JSON_STRING, &t[i], "curTime") == 0) {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            package.curTime = atoi(type_str);
            i++;
        }
        else
        {
            package.sequence = -1;
//            dbgErrorRoutine(ERROR_JSMN_PARSE);
        }
    }

    return package;
}

chain_message parse_chain(char* JSON_STRING)
{
    // terminate at '}'
    int i;
    for (i = 0; i < strlen(JSON_STRING)-1; i ++)
    {
        if (JSON_STRING[i] == '}')
        {
            *(JSON_STRING + i+1) = '\0';
            break;
        }
    }

    chain_message package;
    package.chainNum = CHAIN_NUM;

    int r;
    jsmn_parser p;
    jsmntok_t t[64]; /* We expect no more than 128 tokens */

    jsmn_init(&p);
    r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t) / sizeof(t[0]));

    if (r < 0)
    {
        package.sequence = -1;
//        dbgErrorRoutine(ERROR_JSMN_PARSE);
    }

    /* Assume the top-level element is an object */
    if (r < 1 || t[0].type != JSMN_OBJECT)
    {
        package.sequence = -1;
//        dbgErrorRoutine(ERROR_JSMN_PARSE);
    }

    /* Loop over all keys of the root object */
    for (i = 1; i < r; i++) {
        char type_str[10];

        if (jsoneq(JSON_STRING, &t[i], "value") == 0)
        {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            package.value = atoi(type_str);
            i++;
        }
        else if (jsoneq(JSON_STRING, &t[i], "sequence") == 0)
        {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            package.sequence = atoi(type_str);
            i++;
        }
        else if (jsoneq(JSON_STRING, &t[i], "chainNum") == 0)
        {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
//            package.chainNum = atoi(type_str);
            i++;
        }
        else
        {
            package.sequence = -1;
    //        dbgErrorRoutine(ERROR_JSMN_PARSE);
        }
    }
    return package;
}
