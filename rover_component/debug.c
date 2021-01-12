/*
 * debug.c
 *
 *  Created on: Sep 7, 2020
 *      Author: Spencer
 */


#include "debug.h"

// output a value through GPIOs that says where we are in code
void dbgOutputLoc(unsigned int outLoc)
{
    if (outLoc & GPIO1_MASK)
        GPIO_write(DEBUG_1, 1);
    else
        GPIO_write(DEBUG_1, 0);

    if (outLoc & GPIO2_MASK)
        GPIO_write(DEBUG_2, 1);
    else
        GPIO_write(DEBUG_2, 0);

    if (outLoc & GPIO3_MASK)
        GPIO_write(DEBUG_3, 1);
    else
        GPIO_write(DEBUG_3, 0);

    if (outLoc & GPIO4_MASK)
        GPIO_write(DEBUG_4, 1);
    else
        GPIO_write(DEBUG_4, 0);

    if (outLoc & GPIO5_MASK)
        GPIO_write(DEBUG_5, 1);
    else
        GPIO_write(DEBUG_5, 0);
    // indicates we're ready to read value - everything's set
    // toggle 0 to show change - PIN0 on the digiview
    GPIO_toggle(DEBUG_0);
}

// be more specific - how'd we get here (argument)
void dbgErrorRoutine(int failLoc)
{
    LOG_ERROR("Failing Location: %d", failLoc);
    dbgOutputLoc(failLoc);

    while (1)
    {
        int i;
        for (i = 0; i < 2000000; i ++)
            i ++;

        GPIO_toggle(CONFIG_LED_0_GPIO);
    }
}
