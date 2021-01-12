#include "task2.h"

void ChainCB(char* topic, char* payload)
{
    static int last_seq_recv = 0;

    struct chain_message msg = parse_chain(payload);

    // see if there was a JSON error
    if (msg.sequence == -1)
    {
        dbg_msg.numMissT2 ++;
        last_seq_recv ++; // this should have been a packet
    }
    // no JSON error, we got a good package
    else
    {
        // only forward if it's a legit message
        task2_push(&msg);

        dbg_msg.numRecvT2 ++;

        // add the gap if we missed some sequence numbers
        if (msg.sequence - last_seq_recv - 1 >= 0 && last_seq_recv > 0)
            dbg_msg.numMissT2 += (msg.sequence - last_seq_recv - 1);
        else if (last_seq_recv < 0)
            dbg_msg.numMissT2 ++;


        last_seq_recv = msg.sequence;
    }
}

void *task2Thread(void * arg0)
{
    struct chain_message queueElement;

    LOG_INFO("TASK2 start");

    while(1)
    {
        // message we'll send to the queue
        static struct msgQueue pub_msg;
        pub_msg.event = TASK2_PUB_MSG;

        if (event_connected() == 1)
        {
            // receive from our custom queue
            queueElement = task2_pop();

            if (CHAIN_NUM == SPENCER)
            {
                //unique operation
                queueElement.sequence ++;
                queueElement.value /= 2;
            }
            else if (CHAIN_NUM == MICHAEL)
            {
                //unique operation
                queueElement.sequence ++;
                queueElement.value *= 8;
            }
            else if (CHAIN_NUM == CHARLIE)
            {
                //unique operation
                queueElement.sequence ++;
                queueElement.value /= 4;
            }
            else if (CHAIN_NUM == CHRISTOS)
            {
                //unique operation
                queueElement.sequence ++;
                queueElement.value -= 1;
            }
            // no matter what, publish this.
            // only send message if we got a valid response from encoding
            pub_msg.data_len = encode_chain(pub_msg.payload, queueElement);
            if (pub_msg.data_len > 0 && pub_msg.data_len <= PAYLOAD_SIZE)
                sendMsgToQueue(qh, &pub_msg);
        }
    }
}

void createTask2()
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
    {
        /* failed to set attributes */
        dbgOutputLoc(ERROR_TASK_CREATE);
        dbgErrorRoutine(ERROR_TASK_CREATE);
    }

    retc = pthread_create(&thread, &attrs, task2Thread, NULL);
    if (retc != 0)
    {
        /* pthread_create() failed */
        dbgOutputLoc(ERROR_TASK_CREATE);
        dbgErrorRoutine(ERROR_TASK_CREATE);
    }
}
