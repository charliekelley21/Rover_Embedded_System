/*
 * event_group.c
 *
 *  Created on: Sep 28, 2020
 *      Author: chuck
 */

#include "event_group.h"

void create_event_group() {
    xEventGroup = xEventGroupCreate();

    /* Was the event group created successfully? */
        if( xEventGroup == NULL )
        {
            dbgErrorRoutine(ERROR_EVENT_GROUP);
        }
}
void set_connect_flag(){
    BaseType_t xHigherPriorityTaskWoken;

  /* xHigherPriorityTaskWoken must be initialised to pdFALSE. */
  xHigherPriorityTaskWoken = pdFALSE;

  /* Set bit 0 and bit 4 in xEventGroup. */
  xEventGroupSetBitsFromISR(xEventGroup,   /* The event group being updated. */
                              BIT_0_CONNECT, /* The bits being set. */
                              &xHigherPriorityTaskWoken );
}
void set_task1_v2_flag(){
//    xEventGroupSetBits(xEventGroup, BIT_1_TASK1_V2);
    BaseType_t xHigherPriorityTaskWoken;

  /* xHigherPriorityTaskWoken must be initialised to pdFALSE. */
  xHigherPriorityTaskWoken = pdFALSE;

  /* Set bit 0 and bit 4 in xEventGroup. */
  xEventGroupSetBitsFromISR(xEventGroup,   /* The event group being updated. */
                              BIT_1_TASK1_V2, /* The bits being set. */
                              &xHigherPriorityTaskWoken );
}

void reset_task1_v2_flag(){
    xEventGroupClearBits(xEventGroup, BIT_1_TASK1_V2);
}

int event_connected() {
    EventBits_t uxBits = xEventGroupGetBits(xEventGroup);
    if ((uxBits & BIT_0_CONNECT) == BIT_0_CONNECT) {
        return 1;
    }
    else {
        return -1;
    }
}

int event_v2() {
    EventBits_t uxBits = xEventGroupGetBits(xEventGroup);
    if ((uxBits & BIT_1_TASK1_V2) == BIT_1_TASK1_V2) {
        return 1;
    }
    else {
        return -1;
    }
}
