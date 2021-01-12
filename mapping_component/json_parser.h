#ifndef JSON_PARSE_H_
#define JSON_PARSE_H_

#include <mapping_component.h>

// structs for json parsing
typedef struct debug_msg{
    int sequence;   // sequence number
    int status;     // tell us what this component is doing - will be helpful for monitor
} debug_msg;

static int jsoneq(const char *json, jsmntok_t *tok, const char *s);

int encode_status(char* ret, struct debug_msg pack);

int encode_LIDAR_point(char* ret, char pack[], int end);

#endif /* JSON_PARSE_H_ */
