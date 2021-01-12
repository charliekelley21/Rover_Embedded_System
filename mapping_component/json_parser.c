#include "json_parser.h"

static int jsoneq(const char *json, jsmntok_t *tok, const char *s)
{
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

int encode_status(char* ret, struct debug_msg pack)
{
    return snprintf(ret, PAYLOAD_SIZE, "{\"sequence\": %d, \"status\": %d}", pack.sequence, pack.status);
}

int encode_LIDAR_point(char* ret, char pack[], int end)
{
    uint8_t quality = pack[0] >> 2;
    uint16_t angle = ((pack[2] << 7) + (pack[1] >> 1)); // needs /64 still
    uint16_t dist = ((pack[4] << 8) + pack[3]);    // needs /4 still

    if (end == 1)
        dist = 1; // SIGNAL that we're done with this scan

    return snprintf(ret, PAYLOAD_SIZE, "{\"sequence\": %d, \"q\": %d, \"a\": %d, \"d\": %d}", 0, quality, angle, dist);
}
