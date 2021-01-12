/*
 * task2.h
 *
 *  Created on: Sep 21, 2020
 *      Author: Christos
 */

#ifndef TASK2_H_
#define TASK2_H_

#include "interconnectivity_milestone.h"

void createTask2();
void ChainCB(char* topic, char* payload);
void *task2Thread(void * arg0);

#endif /* TASK2_H_ */
