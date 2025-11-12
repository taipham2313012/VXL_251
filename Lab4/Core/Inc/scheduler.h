/*
 * scheduler.h
 *
 * Created on: 29 Oct 2025
 * Author: PHAM TAN TAI
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h>

// Assuming these are defined somewhere
#define SCH_MAX_TASKS 40
#define TIMER_CYCLE   10 // e.g., 10ms timer tick
#define RETURN_NORMAL 1
#define RETURN_ERROR  0

// Use an index value that is out of bounds
#define SCH_NO_TASK   (SCH_MAX_TASKS)

typedef struct {
    void (* pTask)(void);
    uint32_t Delay;     // Now stores DELTA time from previous task
    uint32_t Period;
    uint8_t  RunMe;
    uint8_t  next;      // Index of next task in the list
} sTask;

// Function prototypes are unchanged
void SCH_Init(void);
void SCH_Update(void);
unsigned char SCH_Add_Task(void (* pFunction)(), uint32_t DELAY, uint32_t PERIOD);
void SCH_Dispatch_Tasks(void);
unsigned char SCH_Delete_Task(const uint8_t TASK_INDEX);

#endif /* INC_SCHEDULER_H_ */
