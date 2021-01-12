#include "task1.h"

// just output the info we recevied
void Task1CB(char* topic, char* payload)
{
    static int last_seq_recv = 0;

    // push results to a queue, needs to hold ~ 15 items
    struct IR_avg msg_in = parseIR_avg(payload);
    task1_v2_push(&msg_in);

    // this means there was a JSON error - bad package!
    if (msg_in.sequence == -1)
        dbg_msg.numMissT1 ++;
    // if there was no JSON error, then we got a good package - make sure we didn't reset
    else if (msg_in.sequence - last_seq_recv > 0)
    {
        dbg_msg.numRecvT1 ++;

        // add the gap if we missed some sequence numbers
        if (msg_in.sequence - last_seq_recv - 1 >= 0 && last_seq_recv > 0)
            dbg_msg.numMissT1 += (msg_in.sequence - last_seq_recv - 1);
        else if (last_seq_recv < 0)
            dbg_msg.numMissT1 ++;

        last_seq_recv = msg_in.sequence;
    }
}

void *task1Thread(void * arg0)
{
    LOG_INFO("TASK1 start");

    init_timers();

    // creating an instance of the state machine struct
    struct StateVars stateMachVars;
    stateMachVars.sequence = 0;
    stateMachVars.curTime = 0;
    stateMachVars.sensorTotal = 0;
    stateMachVars.sensorCount = 0;
    stateMachVars.state = INIT;

    dbgOutputLoc(ENTER_MAIN_TASK);

    // use these in LOOP
    int sensorVal = 0;
    int timeInc = 0;

    if (TASK1_MODE == 0)
    {
        // call once before loop to INIT
        handle_data_state_machine(&stateMachVars, sensorVal, timeInc);
    }

    dbgOutputLoc(BEFORE_WHILE_1);

    // main task LOOP
    while (1)
    {
        if (event_connected() == 1)
        {
            if (TASK1_MODE == 0)
            {
                // receive from a queue, but don't call RTOS directly
                dbgOutputLoc(BEFORE_RECEIVING);
                ReceivedData d = task1_v1_pop();
                dbgOutputLoc(AFTER_RECEIVING);

                if (d.type == 1)
                {
                    sensorVal = d.value;
                    timeInc = 0;
                }
                else if (d.type == 0)
                {
                    timeInc = d.value;
                }
                else
                    dbgOutputLoc(RECEIVE_ERROR);

                // now call the state machine
                handle_data_state_machine(&stateMachVars, sensorVal, timeInc);
            }
            if (event_v2() == 1)
            {
                // publish average if we're a subscriber
                if (TASK1_MODE == 1)
                    publishSensorAverages();

                // either way, publish analytics
                publishAnalytics();

                reset_task1_v2_flag();
            }
        }
    }
}

void createTask1()
{
    // create simple code structure thread
    int retc;
    pthread_t           thread;
    pthread_attr_t      attrs;
    struct sched_param  priParam;

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&attrs);

    /* Set priority, detach state, and stack size attributes */
    priParam.sched_priority = 2;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0)
        dbgErrorRoutine(ERROR_TASK_CREATE);

    retc = pthread_create(&thread, &attrs, task1Thread, NULL);
    if (retc != 0)
        dbgErrorRoutine(ERROR_TASK_CREATE);
}

// This callback is called every 10,000 microseconds, or 10ms
void task1_timerCallback(Timer_Handle myHandle, int_fast16_t status)
{
    static int count_ = 0;
    static TickType_t TICKS = NULL;

    count_ ++;

    if (count_ % 9 == 0)
    {
        GPIO_toggle(CONFIG_GPIO_LED_0); // debug

        if (TASK1_MODE == 0)
        {
            dbgOutputLoc(ISR_90MS_ENTER);

            // TRIGGER 90ms callback - basically, see if we're keeping time
            task1_v1_push(0, xTaskGetTickCountFromISR() - TICKS);
            TICKS = xTaskGetTickCountFromISR();
            dbgOutputLoc(ISR_90MS_EXIT);
        }
    }

    if (count_ % 50 == 0 && TASK1_MODE == 0)
    {
        dbgOutputLoc(ISR_500MS_ENTER);

        // TRIGGER 500ms callback - read from sensor and send value
        task1_v1_push(1, getIRSensorReading(adc));
        dbgOutputLoc(ISR_500MS_EXIT);
    }

    // if we're the receiver and it's been 1 second:
    if (count_ % 100 == 0)
        set_task1_v2_flag();

    if (count_ >= 900)
        count_ = 0;
}

int handle_data_state_machine(StateVars *smVars, int sensorVal, int timeInc)
{
    static struct msgQueue msg;
    msg.event = TASK1_PUB_SENSOR;

    int success = 0;

    switch (smVars->state)
    {
        case INIT:
            // reset everything
            smVars->sequence = 0;
            smVars->curTime = 0;
            smVars->sensorTotal = 0;
            smVars->sensorCount = 0;
            smVars->state = WaitingForTime1;
            success = 1;
            break;
        case WaitingForTime1:
            if (timeInc == 0)
            {
                if (sensorVal != -1)
                {
                    smVars->sensorTotal += sensorVal;
                    smVars->sensorCount += 1;
                }
            }
            else if (timeInc > 0)
            {
                smVars->sequence ++;
                smVars->curTime += timeInc;

                int sensorAvg = smVars->sensorTotal;
                if (smVars->sensorCount != 0)
                    sensorAvg = smVars->sensorTotal/smVars->sensorCount;

                struct IR_avg pack;
                pack.sequence = smVars->sequence;
                pack.type = 1;
                pack.distance = sensorAvg;
                pack.numReadings = smVars->sensorCount;
                pack.curTime = smVars->curTime;

                msg.data_len = encodeIR_avg(msg.payload, pack);

                // only send message if we got a valid response from encoding
                if (msg.data_len > 0 && msg.data_len <= PAYLOAD_SIZE)
                    sendMsgToQueue(qh, &msg);

                smVars->sensorTotal = 0;
                smVars->sensorCount = 0;
                smVars->state = WaitingForTime2;
            }

            success = 1;
            break;
        case WaitingForTime2:
            if (timeInc == 0)
            {
                smVars->sensorTotal += sensorVal;
                smVars->sensorCount += 1;
            }
            else if (timeInc > 0)
            {
                smVars->sequence ++;
                smVars->curTime += timeInc;

                int sensorAvg = 0;
                if (smVars->sensorCount != 0)
                    sensorAvg = smVars->sensorTotal/smVars->sensorCount;

                struct IR_avg pack;
                pack.sequence = smVars->sequence;
                pack.type = 2;
                pack.distance = sensorAvg;
                pack.numReadings = smVars->sensorCount;
                pack.curTime = smVars->curTime;

                msg.data_len = encodeIR_avg(msg.payload, pack);

                // only send message if we got a valid response from encoding
                if (msg.data_len > 0 && msg.data_len <= PAYLOAD_SIZE)
                    sendMsgToQueue(qh, &msg);

                smVars->sensorTotal = 0;
                smVars->sensorCount = 0;
                smVars->state = WaitingForTime3;
            }

            success = 1;
            break;
        case WaitingForTime3:
            if (timeInc == 0)
            {
                smVars->sensorTotal += sensorVal;
                smVars->sensorCount += 1;
            }
            else if (timeInc > 0)
            {
                smVars->sequence ++;
                smVars->curTime += timeInc;

                int sensorAvg = 0;
                if (smVars->sensorCount != 0)
                    sensorAvg = smVars->sensorTotal/smVars->sensorCount;

                struct IR_avg pack;
                pack.sequence = smVars->sequence;
                pack.type = 3;
                pack.distance = sensorAvg;
                pack.numReadings = smVars->sensorCount;
                pack.curTime = smVars->curTime;

                msg.data_len = encodeIR_avg(msg.payload, pack);

                // only send message if we got a valid response from encoding
                if (msg.data_len > 0 && msg.data_len <= PAYLOAD_SIZE)
                    sendMsgToQueue(qh, &msg);

                smVars->sensorTotal = 0;
                smVars->sensorCount = 0;
                smVars->state = WaitingForTime1;
            }

            success = 1;
            break;
        default:
            dbgErrorRoutine(9); // handle error
            success = 0; // return failure?
            break;
    }

    // return success/failure
    return success;
}

void publishAnalytics()
{
    static struct msgQueue pub_msg;
    pub_msg.event = PUB_ANALYTICS;

    // publish the debug analytics too, at 1Hz
    pub_msg.data_len = encode_analytics(pub_msg.payload, dbg_msg);

    // now, send to mqtt thread to be published
    // only send message if we got a valid response from encoding
    if (pub_msg.data_len > 0 && pub_msg.data_len <= PAYLOAD_SIZE)
        sendMsgToQueue(qh, &pub_msg);
}

void publishSensorAverages()
{
    // struct we'll push to the message queue
    static struct msgQueue pub_msg;
    pub_msg.event = TASK1_PUB_AVERAGE;

    static int sequence_t1v2 = 0;
    struct IR_avg queueElement;

    struct Task1_v2_avg msg;
    msg.totMsgs = 0;
    msg.totReads = 0;
    msg.avgRead = 0;
    msg.sequence = sequence_t1v2++;

    int seq = 0;
    while (seq != -1)
    {
        queueElement = task1_v2_pop();
        seq = queueElement.sequence;

        if (seq != -1)
        {
            msg.totMsgs ++;
            msg.totReads += queueElement.numReadings;
            if (queueElement.distance != -1)
                msg.avgRead += queueElement.distance;
        }
    }
    msg.avgRead /= msg.totReads;

    pub_msg.data_len = encodeTask1_v2_avg(pub_msg.payload, msg);

    // now, send to mqtt thread to be published
    // only send message if we got a valid response from encoding
    if (pub_msg.data_len > 0 && pub_msg.data_len <= PAYLOAD_SIZE)
        sendMsgToQueue(qh, &pub_msg);
}

void init_timers()
{
    Timer_Params params;
    Timer_Params_init(&params);
    params.period = 10000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = (Timer_CallBackFxn)task1_timerCallback;

    timer1 = Timer_open(CONFIG_TIMER_1, &params);
    if (timer1 == NULL) {
        /* Failed to initialized timer */
        dbgOutputLoc(TIMER1_OPEN_ERROR);
        dbgErrorRoutine(TIMER1_OPEN_ERROR);
    }
    if (Timer_start(timer1) == Timer_STATUS_ERROR) {
        /* Failed to start timer */
        dbgOutputLoc(TIMER1_START_ERROR);
        dbgErrorRoutine(TIMER1_START_ERROR);
    }
}
