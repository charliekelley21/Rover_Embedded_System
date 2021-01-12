/*
 * event_group.h
 *
 *  Created on: Sep 28, 2020
 *      Author: chuck
 */

#ifndef EVENT_GROUP_H_
#define EVENT_GROUP_H_

#include "FreeRTOS.h"
#include <event_groups.h>
#include "debug.h"

#define BIT_0_CONNECT ( 1 << 0 )

EventGroupHandle_t xEventGroup;

void create_event_group();
void set_connect_flag();

int event_connected();


#endif /* EVENT_GROUP_H_ */
