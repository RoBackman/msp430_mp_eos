#include <msp430.h>
#include "project.h"
#include "sheduler.h"
#include "led.h"


#define SCH_MAX_TASKS 3
#define ERROR_SCH_CANNOT_DELETE_TASK 0x00
#define ERROR_SCH_TOO_MANY_TASKS 0x01
#define RETURN_ERROR 0x01
#define RETURN_NORMAL 0x00


typedef struct {
    void (*pTask)(void);
    uint16_t Delay;
    uint16_t Period;
    uint16_t RunMe;
} sTask;

sTask SCH_tasks_G[SCH_MAX_TASKS];
uint16_t Error_code_G;
uint16_t Last_Error_Code_G;

uint16_t SCH_Delete_Task(const uint16_t TASK_INDEX){
    uint16_t Return_Code;

    if (SCH_tasks_G[TASK_INDEX].pTask == 0){
        //No task found set global error variable
        Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;

        Return_Code = RETURN_ERROR;
    }
    else {
        Return_Code = RETURN_NORMAL;
    }

    //Delete the task
    SCH_tasks_G[TASK_INDEX].pTask = 0x0000;
    SCH_tasks_G[TASK_INDEX].Delay = 0;
    SCH_tasks_G[TASK_INDEX].Period = 0;
    SCH_tasks_G[TASK_INDEX].RunMe = 0;

    //Return status
    return Return_Code;
}

uint16_t SCH_Add_Task(void (*pFunction) (), const uint16_t DELAY, const uint16_t PERIOD){
    uint16_t Index = 0;

    //Find first free gap in array (if there is one)
    while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS)){
        Index++;
    }

    //Have we reached the end of the list
    if (Index == SCH_MAX_TASKS){
        //List is full set global error variable
        Error_code_G = ERROR_SCH_TOO_MANY_TASKS;

        return SCH_MAX_TASKS;
    }

    //If there is still space in the array 
    SCH_tasks_G[Index].pTask = pFunction;
    SCH_tasks_G[Index].Delay = DELAY;
    SCH_tasks_G[Index].Period = PERIOD;
    SCH_tasks_G[Index].RunMe = 0;

    //Return position of task to allow deletion
    return Index;
}

void SCH_Report_Status(void){
    //Only do this if we want to report errors
    #ifdef SCH_REPORT_ERRORS
    //Check for new error code
    if (Error_code_G != Last_Error_Code_G){
        Error_Port = 255 - Error_code_G;

        Last_Error_Code_G = Error_code_G;

        if (Error_code_G != 0){
            Error_Tick_Count_G = 60000;
        }
        else {
            Error_Tick_Count_G = 0;
        }
    }
    else {
        if (Error_Tick_Count_G != 0){
            if (--Error_Tick_Count_G == 0){
                //Reset error code
                Error_code_G = 0;
            }
        }
    }

    #endif
}

void SCH_Go_To_Sleep(void){
    // Enter LPM1
    __bis_SR_register(LPM1_bits + GIE);
}

void SCH_Init(void){
    uint16_t Index;

    //Clear task list
    for (Index = 0; Index < SCH_MAX_TASKS; Index++){
        SCH_Delete_Task(Index);
    }

    Error_code_G = 0;

	//Set frequency to 16 MHz
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL = CALDCO_16MHZ;

	//Set up timer 0
	CCTL0 = CCIE;
	TACTL = TASSEL_2 + MC_1 + ID_3;

	//Set compare value
	CCR0 = TMRC;

	//Enable interrupts
	__enable_interrupt();
}

__attribute__((interrupt(TIMER0_A0_VECTOR))) void SCH_Update(void) {
    uint16_t Index;

    for (Index = 0; Index < SCH_MAX_TASKS; Index++){
        //Check if any task att this position in list
        if (SCH_tasks_G[Index].pTask){
            //Update task tick and check if task is due to run
            if (SCH_tasks_G[Index].Delay-- == 0){
                //Increment the run me counter
                SCH_tasks_G[Index].RunMe += 1;
                //Check if task is periodic
                if (SCH_tasks_G[Index].Period){
                    //Shedule task to run again
                    SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
                }
            }
        }
    }
}

void SCH_Dispatch_Tasks(void){
    uint16_t Index;

    for (Index = 0; Index < SCH_MAX_TASKS; Index++){
        //Check if task should be run
        if (SCH_tasks_G[Index].RunMe > 0){
            //Run the task
            SCH_tasks_G[Index].pTask();
            //Decrement the run me counter
            SCH_tasks_G[Index].RunMe -= 1;
            
            //If task is not periodic delete it
            if (SCH_tasks_G[Index].Period == 0){
                SCH_Delete_Task(Index);
            }
        }
    }

    //Report system status
    //SCH_Report_Status();

    //Enter sleep mode
    //SCH_Go_To_Sleep();
}


