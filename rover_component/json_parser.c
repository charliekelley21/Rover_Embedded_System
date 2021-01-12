#include "json_parser.h"

static int jsoneq(const char *json, jsmntok_t *tok, const char *s)
{
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

int encode_state_mqtt(char* ret, int state, int project_state) {
    return snprintf(ret, PAYLOAD_SIZE, "{\"state\": %d, \"project state\": %d}", state, project_state);
}

int encode_odometry(char* ret, float d_distance, float d_theta, float arm_rotation) {
    return snprintf(ret, PAYLOAD_SIZE, "{\"distance\": %f, \"theta_move\": %f, \"theta_arm\": %f}", d_distance, d_theta, arm_rotation);
}

int encode_state(char* ret, struct state_t state) {
    return snprintf(ret, PAYLOAD_SIZE, "{\"sequence\": %d, \"distance\": %f, \"theta\": %f, \"identity\": %d}", state.sequence, state.distance, state.theta, state.identity);
}

state_t parse_state(char* JSON_STRING) {
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

    state_t state;
    int r;
    jsmn_parser p;
    jsmntok_t t[64]; /* We expect no more than 128 tokens */

    jsmn_init(&p);
    r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t) / sizeof(t[0]));

    if (r < 0)
    {
//        dbgErrorRoutine(ERROR_JSMN_PARSE);
    }

    /* Assume the top-level element is an object */
    if (r < 1 || t[0].type != JSMN_OBJECT)
    {
//        dbgErrorRoutine(ERROR_JSMN_PARSE);
    }

    /* Loop over all keys of the root object */
    for (i = 1; i < r; i++) {

        char type_str[10];
        if (jsoneq(JSON_STRING, &t[i], "sequence") == 0) {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            state.sequence = atoi(type_str);
            i++;
        }
        else if (jsoneq(JSON_STRING, &t[i], "distance") == 0) {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            state.distance = atof(type_str);
            i++;
        }
        else if (jsoneq(JSON_STRING, &t[i], "theta") == 0) {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            state.theta = atof(type_str);
            i++;
        }
        else if (jsoneq(JSON_STRING, &t[i], "identity") == 0) {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            state.identity = atoi(type_str);
            i++;
        }
        else
        {
            dbgErrorRoutine(ERROR_JSMN_PARSE);
        }
    }

    return state;
}

int encode_control(char* ret, struct controller_t control) {
    return snprintf(ret, PAYLOAD_SIZE, "{\"sequence\": %d, \"index\": %d, \"direction\": %d, \"current speed\": %d, \"desired speed\": %d, \"integral\": %f, \"error\": %f}", control.sequence, control.index, control.direction, control.current_speed, control.desired_speed, control.integral, control.error);
}

controller_t parse_control(char* JSON_STRING) {
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

    controller_t control;
    int r;
    jsmn_parser p;
    jsmntok_t t[64]; /* We expect no more than 128 tokens */

    jsmn_init(&p);
    r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t) / sizeof(t[0]));

    if (r < 0)
    {
//        dbgErrorRoutine(ERROR_JSMN_PARSE);
    }

    /* Assume the top-level element is an object */
    if (r < 1 || t[0].type != JSMN_OBJECT)
    {
//        dbgErrorRoutine(ERROR_JSMN_PARSE);
    }

    /* Loop over all keys of the root object */
    for (i = 1; i < r; i++) {

        char type_str[10];
        if (jsoneq(JSON_STRING, &t[i], "current_speed") == 0) {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            control.current_speed = atoi(type_str);
            i++;
        }
        else if (jsoneq(JSON_STRING, &t[i], "index") == 0) {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            control.index = atoi(type_str);
            i++;
        }
        else if (jsoneq(JSON_STRING, &t[i], "sequence") == 0) {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            control.sequence = atoi(type_str);
            i++;
        }
        else if (jsoneq(JSON_STRING, &t[i], "desired_speed") == 0) {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            control.desired_speed = atoi(type_str);
            i++;
        }
        else if (jsoneq(JSON_STRING, &t[i], "integral") == 0) {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            control.integral = atof(type_str);
            i++;
        }
        else if (jsoneq(JSON_STRING, &t[i], "direction") == 0) {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            control.direction = atoi(type_str);
            i++;
        }
        else if (jsoneq(JSON_STRING, &t[i], "error") == 0) {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            control.error = atof(type_str);
            i++;
        }
        else
        {
//            dbgErrorRoutine(ERROR_JSMN_PARSE);
        }
    }

    return control;
}


int encode_SPI(char* ret, struct wheel_t wheel){
    return snprintf(ret, PAYLOAD_SIZE, "{\"sequence\": %d, \"index\": %d, \"current position\": %d, \"previous position\": %d}", wheel.sequence, wheel.index, wheel.pos, wheel.prev_pos);
}

wheel_t parse_spi(char* JSON_STRING) {
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

    wheel_t wheel;
    int r;
    jsmn_parser p;
    jsmntok_t t[64]; /* We expect no more than 128 tokens */

    jsmn_init(&p);
    r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t) / sizeof(t[0]));

    if (r < 0)
    {
        wheel.sequence = -1;
//        dbgErrorRoutine(ERROR_JSMN_PARSE);
    }

    /* Assume the top-level element is an object */
    if (r < 1 || t[0].type != JSMN_OBJECT)
    {
        wheel.sequence = -1;
//        dbgErrorRoutine(ERROR_JSMN_PARSE);
    }

    /* Loop over all keys of the root object */
    for (i = 1; i < r; i++) {

        char type_str[10];
        if (jsoneq(JSON_STRING, &t[i], "sequence") == 0) {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            wheel.sequence = atoi(type_str);
            i++;
        }
        else if (jsoneq(JSON_STRING, &t[i], "index") == 0) {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            wheel.index = atoi(type_str);
            i++;
        }
        else if (jsoneq(JSON_STRING, &t[i], "current position") == 0) {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            wheel.pos = atoi(type_str);
            i++;
        }
        else if (jsoneq(JSON_STRING, &t[i], "previous position") == 0) {
            sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
            wheel.prev_pos = atoi(type_str);
            i++;
        }
        else
        {
            wheel.sequence = -1;
//            dbgErrorRoutine(ERROR_JSMN_PARSE);
        }
    }

    return wheel;
}

int encode_move(char* ret, struct move_t move) {
    return snprintf(ret, PAYLOAD_SIZE, "{\"sequence\": %d, \"distance\": %f, \"theta\": %f}", move.sequence, move.distance, move.theta);
}

move_t parse_move(char* JSON_STRING) {
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

        move_t move;
        int r;
        jsmn_parser p;
        jsmntok_t t[64]; /* We expect no more than 128 tokens */

        jsmn_init(&p);
        r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t) / sizeof(t[0]));

        if (r < 0)
        {
            move.sequence = -1;
    //        dbgErrorRoutine(ERROR_JSMN_PARSE);
        }

        /* Assume the top-level element is an object */
        if (r < 1 || t[0].type != JSMN_OBJECT)
        {
            move.sequence = -1;
    //        dbgErrorRoutine(ERROR_JSMN_PARSE);
        }

        /* Loop over all keys of the root object */
        for (i = 1; i < r; i++) {

            char type_str[10];
            if (jsoneq(JSON_STRING, &t[i], "sequence") == 0) {
                sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
                move.sequence = atoi(type_str);
                i++;
            }
            else if (jsoneq(JSON_STRING, &t[i], "distance") == 0) {
                sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
                move.distance = atof(type_str);
                i++;
            }
            else if (jsoneq(JSON_STRING, &t[i], "theta") == 0) {
                sprintf(type_str, "%.*s", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
                move.theta = atof(type_str);
                i++;
            }
            else if (jsoneq(JSON_STRING, &t[i], "identity") == 0) {
                i++; //skip identity, used only for state variable
            }
            else
            {
                move.sequence = -1;
    //            dbgErrorRoutine(ERROR_JSMN_PARSE);
            }
        }

        return move;
}
