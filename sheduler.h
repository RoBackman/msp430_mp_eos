#include "project.h"

uint16_t SCH_Delete_Task(const uint16_t TASK_INDEX);
uint16_t SCH_Add_Task(void (*pFunction) (), const uint16_t DELAY, const uint16_t PERIOD);
void SCH_Report_Status(void);
void SCH_Go_To_Sleep(void);
void SCH_Init(void);

__attribute__((interrupt(TIMER0_A0_VECTOR))) void Timer_A(void);

void SCH_Update(void);
void SCH_Dispatch_Tasks(void);