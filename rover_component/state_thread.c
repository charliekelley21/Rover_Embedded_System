/*
 * state_thread.c
 *
 *  Created on: Oct 28, 2020
 *      Author: chuck
 */

#include "state_thread.h"

void create_state_task() {
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

    retc = pthread_create(&thread, &attrs, state_thread, NULL);
    if (retc != 0)
        dbgErrorRoutine(ERROR_TASK_CREATE);
}

void *state_thread(void *arg0){
    while (event_connected() == -1) {
            ;
        }

    struct state_t temp = {0, 0.0, 0.0, 0};
    arm_adjust();

    fsm_rover(temp);

    while (1) {
        if (state_pop(&temp)) {
            fsm_rover(temp);
        }
    }
}

void publish_movement(struct move_t odom, float arm_rotation) {
    struct msgQueue msg;
    msg.event = ODOM_MSG;
    msg.data_len = encode_odometry(msg.payload, odom.distance, odom.theta, arm_rotation);
    if (msg.data_len > 0 && msg.data_len <= PAYLOAD_SIZE)
        broker_push(&msg);
}

void arm_adjust() {
    struct move_t ref = {0, 0.05, 0};
    movement_push(ref);
}

void move_rover(struct move_t move, int odom) {

    static struct move_t ref = {0, 0.0, 0.0};

    ref.sequence++;
    ref.theta = move.theta;
    ref.distance = 0.0;
    movement_push(ref);

    ref.sequence++;
    ref.distance = move.distance;
    ref.theta = 0.0;
    movement_push(ref);


}

int fsm_rover(struct state_t state_msg) {
    static int state = INIT;
    static int project_state = EMPTY;
    static struct move_t delta_movement = {0, 0.0, 0.0};
    static struct move_t arm_orientation = {0, 0.0, 2.0};
    static float arm_rotation = 0.0;

    int success = 1;

    if (state_msg.identity == ARM_DROP) {
        state = ARM_DROPPED;

    }

    switch (state) {
    case INIT:                                                          //---INIT
        publish_movement(delta_movement, arm_rotation);                 // publish init 0,0,0 or last arm movement
        state = WAITING;                                                // state -> waiting
        break;
    case WAITING:                                                       //---WAITING
        if (((state_msg.distance != 0.) || (state_msg.theta != 0.)) && (state_msg.identity < 3)) {      // branch: is movement provided
            delta_movement.distance = state_msg.distance;               // store distance
            delta_movement.theta = state_msg.theta;                     // store rotation
            state += state_msg.identity + 1;                            // state -> corresponding to id
            state_push(state_msg);
        }
        break;
    case MOVE_OBJ:                                                      //---MOVE_OBJ
        arm_rotation = ARM_ROTATE;
        move_rover(delta_movement, 0);                                     // move rover
        project_state = LOADED;                                         // arm will be full for next move
        state = ARM_ORIENT;                                             // state -> rotate to arm
        state_push(state_msg);                                          // push dummy msg to move through next state
        break;
    case MOVE_DROPOFF:                                                  //---MOVE_DROPOFF
        arm_rotation = ARM_ROTATE;
        move_rover(delta_movement, 0);                                     // move rover
        project_state = COMPLETED;                                      // drop off complete
        state = ARM_ORIENT;                                             // state -> rotate to arm
        state_push(state_msg);                                          // push dummy msg to move through next state
        break;
    case MOVE_FRONT:                                                    //---MOVE_FRONT
        arm_rotation = 0.0;
        move_rover(delta_movement, 1);                                     // move rover
        state = POST_ODOMETRY;
//        state_push(state_msg);                                          // push dummy msg to move through next state
        break;
    case ARM_ORIENT:                                                    //---ARM_ORIENT
        move_rover(arm_orientation, 0);                                     // move rover
        state = ARM_WAIT;
        state_push(state_msg);                                          // push dummy msg to move through next state
        break;
    case ARM_WAIT:                                                      //---ARM_WAIT
        if (state_msg.identity == ARM_FINISH_OBJ || state_msg.identity == ARM_FINISH_DROP) {
                                                                        // check messages for arm finished
            state = POST_ODOMETRY;                                      // state -> odometry
            state_push(state_msg);
        }
        else if (state_msg.identity == ARM_OUT_OF_REACH) {
            state = ARM_ADJUST;
            state_push(state_msg);
        }
        break;
    case ARM_ADJUST:
        arm_adjust();
        state = ARM_WAIT;
        break;
    case ARM_DROPPED:
        project_state = EMPTY;
        state = ARM_WAIT;                                          // state -> odometry
        break;
    case POST_ODOMETRY:                                                 //---POST_ODOMETRY
        if (project_state == COMPLETED) {                               // check if object has been recovered
            state = FINISH;                                             // move to wait for project cycle
        }
        else {
            publish_movement(delta_movement, arm_rotation);             // publish movement if not done
                                                                        //      recovering object
            state = WAITING;
        }
        break;
    case FINISH:                                                        //---FINISH
        if (state_msg.identity == RESTART_ID) {                         // id to start looking for a new object
            state = INIT;                                               // state -> init
        }
        break;
    default:                                                            //---DEFAULT
        dbgErrorRoutine(ERROR_STATE_MACHINE);                           // fail all
        success = 0;
        break;
    }

    static struct msgQueue msg;
    msg.event = STATE_MSG;
    msg.data_len = encode_state_mqtt(msg.payload, state, project_state);
    if (msg.data_len > 0 && msg.data_len <= PAYLOAD_SIZE)
        broker_push(&msg);

    return success;
}
