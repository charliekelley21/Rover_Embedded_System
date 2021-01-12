#include <mapping_component.h>

extern void * mainThread(void *arg0);

int main(void)
{
    pthread_t thread;
    pthread_attr_t pAttrs;
    struct sched_param priParam;
    int retc;
    int detachState;

    create_event_group();

    /* Call board init functions */
    Board_init();

    /* Set priority and stack size attributes */
    pthread_attr_init(&pAttrs);
    priParam.sched_priority = 2;

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&pAttrs, detachState);
    if(retc != 0)
        dbgErrorRoutine(ERROR_TASK_CREATE);

    pthread_attr_setschedparam(&pAttrs, &priParam);

    retc |= pthread_attr_setstacksize(&pAttrs, THREADSTACKSIZE);
    if(retc != 0)
        dbgErrorRoutine(ERROR_TASK_CREATE);

    retc = pthread_create(&thread, &pAttrs, mainThread, NULL);
    if(retc != 0)
        dbgErrorRoutine(ERROR_TASK_CREATE);

    create_queues();
    create_task_queues();

    // call our function to create the task to read from IR with state machine
    taskLIDAR_create();
    taskUART_read_create();

    // store our debugging stats
    dbg_msg.sequence = 0;
    dbg_msg.status = 0;

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    return (0);
}

//! \brief Application defined malloc failed hook
void vApplicationMallocFailedHook()
{
    dbgErrorRoutine(ERROR_RTOS_MALLOC);
}

//! \brief Application defined stack overflow hook
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    dbgErrorRoutine(ERROR_RTOS_STACK_OVERFLOW);
}

void vApplicationTickHook(void)
{
    /*
     * This function will be called by each tick interrupt if
     * configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
     * added here, but the tick hook is called from an interrupt context, so
     * code must not attempt to block, and only the interrupt safe FreeRTOS API
     * functions can be used (those that end in FromISR()).
     */
}

void vPreSleepProcessing(uint32_t ulExpectedIdleTime)
{
}

//! \brief Application defined idle task hook
void vApplicationIdleHook(void)
{
    /* Handle Idle Hook for Profiling, Power Management etc */
}

//! \brief  Overwrite the GCC _sbrk function which check the heap limit related
//!         to the stack pointer.
//!         In case of freertos this checking will fail.
#if defined (__GNUC__)
void * _sbrk(uint32_t delta)
{
    extern char _end;     /* Defined by the linker */
    extern char __HeapLimit;
    static char *heap_end;
    static char *heap_limit;
    char *prev_heap_end;

    if(heap_end == 0)
    {
        heap_end = &_end;
        heap_limit = &__HeapLimit;
    }

    prev_heap_end = heap_end;
    if(prev_heap_end + delta > heap_limit)
    {
        return((void *) -1L);
    }
    heap_end += delta;
    return((void *) prev_heap_end);
}

#endif
