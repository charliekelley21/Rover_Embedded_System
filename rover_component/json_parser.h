#ifndef JSON_PARSE_H_
#define JSON_PARSE_H_

#include "rover_component.h"
#include "jsmn.h"
#include "math.h"

// structs for json parsing

typedef struct wheel_t{
    int sequence;
    int index;
    int pos;
    int prev_pos;
} wheel_t;

typedef struct controller_t{
    int sequence;
    int index;
    int direction;
    int current_speed;
    int desired_speed;
    float integral;
    float error;
} controller_t;

typedef struct move_t{
    int sequence;
    float distance;     // meters
    float theta;        // radians
} move_t;

typedef struct state_t{
    int sequence;
    float distance;
    float theta;
    int identity;
} state_t;

#define PAYLOAD_SIZE        512

static int jsoneq(const char *json, jsmntok_t *tok, const char *s);

int encode_state_mqtt(char* ret, int state, int project_state);
int encode_odometry(char* ret, float d_distance, float d_theta, float arm_rotation);

int encoder_state(char* ret, struct state_t state);
state_t parse_state(char* JSON_STRING);

int encode_SPI(char* ret, struct wheel_t wheel);
wheel_t parse_spi(char* JSON_STRING);

int encode_control(char* ret, struct controller_t control);
controller_t parse_control(char* JSON_STRING);

int encode_move(char* ret, struct move_t move);
move_t parse_move(char* JSON_STRING);

#endif /* JSON_PARSE_H_ */
