/*
 * event_group.h
 *
 *  Created on: Sep 28, 2020
 *      Author: chuck
 */

#ifndef EVENT_GROUP_H_
#define EVENT_GROUP_H_

#include "interconnectivity_milestone.h"

#define BIT_0_CONNECT ( 1 << 0 )
#define BIT_1_TASK1_V2 ( 1 << 1 )

EventGroupHandle_t xEventGroup;

void create_event_group();
void set_connect_flag();
void set_task1_v2_flag();
void reset_task1_v2_flag();

int event_connected();
int event_v2();


#endif /* EVENT_GROUP_H_ */
