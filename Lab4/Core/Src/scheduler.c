/*
 * scheduler.c
 *
 * Created on: 29 Oct 2025
 * Author: PHAM TAN TAI
 */

#include "scheduler.h"

// The task array, now used as a "pool"
sTask SCH_tasks_G[SCH_MAX_TASKS];

// List heads
static uint8_t g_WaitingListHead = SCH_NO_TASK;
static uint8_t g_FreeListHead = SCH_NO_TASK;

/**
 * @brief Gets a free task struct from the free list.
 * @return Index of the task, or SCH_NO_TASK if the list is full.
 */
static uint8_t SCH_Get_Free_Task_Index(void) {
    if (g_FreeListHead == SCH_NO_TASK) {
        return SCH_NO_TASK; // No free tasks
    }

    uint8_t freeIndex = g_FreeListHead;
    g_FreeListHead = SCH_tasks_G[freeIndex].next;
    return freeIndex;
}

/**
 * @brief Returns a task struct to the free list.
 * @param TASK_INDEX The index to return.
 */
static void SCH_Return_Task_To_Free_List(const uint8_t TASK_INDEX) {
    // Clear the task struct
    SCH_tasks_G[TASK_INDEX].pTask = 0x0000;
    SCH_tasks_G[TASK_INDEX].Delay = 0;
    SCH_tasks_G[TASK_INDEX].Period = 0;
    SCH_tasks_G[TASK_INDEX].RunMe = 0;

    // Add to head of free list
    SCH_tasks_G[TASK_INDEX].next = g_FreeListHead;
    g_FreeListHead = TASK_INDEX;
}

void SCH_Init(void) {
    uint8_t i;
    // Link all tasks into the free list
    for (i = 0; i < SCH_MAX_TASKS; i++) {
        // Return task to free list (which also clears it)
        SCH_Return_Task_To_Free_List(i);
    }
    g_WaitingListHead = SCH_NO_TASK; // Waiting list is empty
}

void SCH_Update(void) {
    // No tasks in the waiting list
    if (g_WaitingListHead == SCH_NO_TASK) {
        return;
    }

    // Decrement the delay of the head task
    if (SCH_tasks_G[g_WaitingListHead].Delay > 0) {
        SCH_tasks_G[g_WaitingListHead].Delay--;
    }

    // Check if the head task is due (and loop for multiple tasks)
    while (g_WaitingListHead != SCH_NO_TASK && SCH_tasks_G[g_WaitingListHead].Delay == 0) {
        uint8_t taskIndex = g_WaitingListHead;

        // Task is due
        SCH_tasks_G[taskIndex].RunMe = 1;

        // Move head to the next task
        g_WaitingListHead = SCH_tasks_G[taskIndex].next;

        if (SCH_tasks_G[taskIndex].Period > 0) {
            // Re-add periodic task
            // Note: This is an O(k) operation
            SCH_Add_Task(SCH_tasks_G[taskIndex].pTask,
                         SCH_tasks_G[taskIndex].Period,
                         SCH_tasks_G[taskIndex].Period);
        }

        // One-shot tasks are not re-added.
        // SCH_Dispatch_Tasks will see Period == 0 and call
        // SCH_Delete_Task, which will return the struct to the free list.
    }
}

unsigned char SCH_Add_Task(void (* pFunction)(), uint32_t DELAY, uint32_t PERIOD) {

    uint8_t newTaskIndex = SCH_Get_Free_Task_Index();

    // Have we reached the end of the list?
    if (newTaskIndex == SCH_NO_TASK) {
        // Task list is full
        return SCH_MAX_TASKS; // Return error code
    }

    // If we're here, there is a space in the task array
    SCH_tasks_G[newTaskIndex].pTask = pFunction;
    SCH_tasks_G[newTaskIndex].Period = PERIOD;
    SCH_tasks_G[newTaskIndex].RunMe = 0;

    // Calculate delay in ticks
    uint32_t delay_in_ticks = DELAY / TIMER_CYCLE;

    uint8_t current = g_WaitingListHead;
    uint8_t previous = SCH_NO_TASK;
    uint32_t cumulative_delay = 0;

    // --- Begin Sorted Insert (O(k)) ---

    // Case 1: Waiting list is empty
    if (g_WaitingListHead == SCH_NO_TASK) {
        g_WaitingListHead = newTaskIndex;
        SCH_tasks_G[newTaskIndex].next = SCH_NO_TASK;
        SCH_tasks_G[newTaskIndex].Delay = delay_in_ticks;
        return newTaskIndex;
    }

    // Iterate the waiting list to find the correct insertion point
    while (current != SCH_NO_TASK) {
        uint32_t current_task_delay = SCH_tasks_G[current].Delay;

        if (delay_in_ticks < cumulative_delay + current_task_delay) {
            // Found insertion point: insert *before* current
            break;
        }
        cumulative_delay += current_task_delay;
        previous = current;
        current = SCH_tasks_G[current].next;
    }

    // Case 2: Insert at head (previous is SCH_NO_TASK)
    if (previous == SCH_NO_TASK) {
        g_WaitingListHead = newTaskIndex;
        SCH_tasks_G[newTaskIndex].next = current;
        SCH_tasks_G[newTaskIndex].Delay = delay_in_ticks;

        // Adjust the old head's delay
        SCH_tasks_G[current].Delay -= delay_in_ticks;
    }
    // Case 3: Insert in middle or at end
    else {
        SCH_tasks_G[previous].next = newTaskIndex;
        SCH_tasks_G[newTaskIndex].next = current;
        SCH_tasks_G[newTaskIndex].Delay = delay_in_ticks - cumulative_delay;

        // If not at end, adjust the next task's delay
        if (current != SCH_NO_TASK) {
            SCH_tasks_G[current].Delay -= SCH_tasks_G[newTaskIndex].Delay;
        }
    }
    // --- End Sorted Insert ---

    return newTaskIndex;
}

void SCH_Dispatch_Tasks(void) {
    unsigned char Index;

    // Dispatches (runs) the next task (if one is ready)
    // This is still O(n) because it checks the entire array pool
    for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
        if (SCH_tasks_G[Index].RunMe > 0) {
            (*SCH_tasks_G[Index].pTask)(); // Run the task
            SCH_tasks_G[Index].RunMe = 0; // Reset RunMe flag

            SCH_Delete_Task(Index);

        }
    }
}

unsigned char SCH_Delete_Task(const uint8_t TASK_INDEX) {
    if (TASK_INDEX >= SCH_MAX_TASKS) {
        return RETURN_ERROR;
    }

    // Task must exist to be deleted
    if (SCH_tasks_G[TASK_INDEX].pTask == 0) {
        return RETURN_ERROR;
    }

    uint8_t current = g_WaitingListHead;
    uint8_t previous = SCH_NO_TASK;

    // --- Find and remove task from waiting list (O(k)) ---
    while (current != SCH_NO_TASK) {
        if (current == TASK_INDEX) {
            // Found it. Now remove it.
            uint8_t next_task = SCH_tasks_G[current].next;

            // Case 1: Deleting the head
            if (previous == SCH_NO_TASK) {
                g_WaitingListHead = next_task;
            }
            // Case 2: Deleting from middle/end
            else {
                SCH_tasks_G[previous].next = next_task;
            }

            // Case 3: Patch the next task's delay
            if (next_task != SCH_NO_TASK) {
                SCH_tasks_G[next_task].Delay += SCH_tasks_G[current].Delay;
            }

            // Task is removed from waiting list. Now return to free list.
            SCH_Return_Task_To_Free_List(TASK_INDEX);
            return RETURN_NORMAL;
        }

        previous = current;
        current = SCH_tasks_G[current].next;
    }

    // Task was not found in the waiting list.
    if (SCH_tasks_G[TASK_INDEX].pTask != 0) {
        SCH_Return_Task_To_Free_List(TASK_INDEX);
        return RETURN_NORMAL;
    }

    return RETURN_ERROR;
}

// END
