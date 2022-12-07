/*
 * scheduler.c
 *
 *  Created on: Nov 11, 2022
 *      Author: HUY
 */
#include "scheduler.h"

sTasks SCH_tasks_G[SCH_MAX_TASKS];
uint8_t current_index_task = 0;

void SCH_Init(void){
	current_index_task = 0;
}

uint32_t SCH_Add_Task ( void (*pFunction)() , uint32_t DELAY, uint32_t PERIOD){
	if(current_index_task < SCH_MAX_TASKS){

		SCH_tasks_G[current_index_task].pTask = pFunction;
		SCH_tasks_G[current_index_task].Delay = DELAY;
		SCH_tasks_G[current_index_task].Period =  PERIOD;
		SCH_tasks_G[current_index_task].RunMe = 0;

		SCH_tasks_G[current_index_task].TaskID = current_index_task;
		current_index_task++;
		return SCH_tasks_G[current_index_task - 1].TaskID;
	}
	return SCH_MAX_TASKS;
}


void SCH_Update(void){
    uint8_t Index;
    uint8_t count = 0;
    for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
    	if (count == current_index_task)
    		return;
        if (SCH_tasks_G[Index].pTask){
            if (SCH_tasks_G[Index].Delay == 0) {
                SCH_tasks_G[Index].RunMe += 1;
                if (SCH_tasks_G[Index].Period) {
                    SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
                }
            } else {
                SCH_tasks_G[Index].Delay -= 1;
            }
        }
        count++;
    }
}

void SCH_Dispatch_Tasks(void){
    uint8_t Index;
    uint8_t count = 0;
    for (Index = 0; Index < SCH_MAX_TASKS; Index++){
    	if (count == current_index_task)
    		return;
        if (SCH_tasks_G[Index].RunMe > 0) {
            (*SCH_tasks_G[Index].pTask)();
            SCH_tasks_G[Index].RunMe -= 1;

            if (SCH_tasks_G[Index].Period == 0)
            {
                SCH_Delete_Task(SCH_tasks_G[Index].TaskID);
            }
        }
        count++;
    }
}

uint8_t SCH_Delete_Task(uint32_t taskID){
    if (SCH_tasks_G[taskID].pTask == 0) {
        return 1;
    }
    SCH_tasks_G[taskID].pTask = 0x0000;
    SCH_tasks_G[taskID].Delay = 0;
    SCH_tasks_G[taskID].Period = 0;
    SCH_tasks_G[taskID].RunMe = 0;
    SCH_tasks_G[taskID].TaskID = 0;
    return 0;
}
