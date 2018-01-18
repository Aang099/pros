#pragma once

#include "rtos/FreeRTOS.h"
#include "rtos/list.h"

/*
 * Task control block.  A task control block (TCB) is allocated for each task,
 * and stores task state information, including a pointer to the task's context
 * (the task's run time environment, including register values)
 */
typedef struct tskTaskControlBlock {
	volatile task_stack_t* pxTopOfStack; /*< Points to the location of the last item placed on the tasks stack.
	                                        THIS MUST BE THE FIRST MEMBER OF THE TCB STRUCT. */

	list_item_t xStateListItem; /*< The list that the state list item of a task is reference from denotes the state
	                               of that task (Ready, Blocked, Suspended ). */
	list_item_t xEventListItem; /*< Used to reference a task from an event list. */
	uint32_t uxPriority;        /*< The priority of the task.  0 is the lowest priority. */
	task_stack_t* pxStack;      /*< Points to the start of the stack. */
	char pcTaskName[configMAX_TASK_NAME_LEN];
	    /*< Descriptive name given to the task when created.  Facilitates debugging only. */ /*lint !e971
	                                                                                            Unqualified char
	                                                                                            types are allowed
	                                                                                            for strings and
	                                                                                            single characters
	                                                                                            only. */

#if (portSTACK_GROWTH > 0)
	task_stack_t* pxEndOfStack; /*< Points to the end of the stack on architectures where the stack grows up from
	                               low memory. */
#endif

#if (portCRITICAL_NESTING_IN_TCB == 1)
	uint32_t uxCriticalNesting; /*< Holds the critical section nesting depth for ports that do not maintain their
	                               own count in the port layer. */
#endif

#if (configUSE_TRACE_FACILITY == 1)
	uint32_t uxTCBNumber;  /*< Stores a number that increments each time a TCB is created.  It allows debuggers to
	                          determine when a task has been deleted and then recreated. */
	uint32_t uxTaskNumber; /*< Stores a number specifically for use by third party trace code. */
#endif

#if (configUSE_MUTEXES == 1)
	uint32_t
	    uxBasePriority; /*< The priority last assigned to the task - used by the priority inheritance mechanism. */
	uint32_t uxMutexesHeld;
#endif

#if (configUSE_APPLICATION_TASK_TAG == 1)
	TaskHookFunction_t pxTaskTag;
#endif

#if (configNUM_THREAD_LOCAL_STORAGE_POINTERS > 0)
	void* pvThreadLocalStoragePointers[configNUM_THREAD_LOCAL_STORAGE_POINTERS];
#endif

#if (configGENERATE_RUN_TIME_STATS == 1)
	uint32_t ulRunTimeCounter; /*< Stores the amount of time the task has spent in the Running state. */
#endif

#if (configUSE_NEWLIB_REENTRANT == 1)
	/* Allocate a Newlib reent structure that is specific to this task.
	Note Newlib support has been included by popular demand, but is not
	used by the FreeRTOS maintainers themselves.  FreeRTOS is not
	responsible for resulting newlib operation.  User must be familiar with
	newlib and must provide system-wide implementations of the necessary
	stubs. Be warned that (at the time of writing) the current newlib design
	implements a system-wide malloc() that must be provided with locks. */
	struct _reent xNewLib_reent;
#endif

#if (configUSE_TASK_NOTIFICATIONS == 1)
	volatile uint32_t ulNotifiedValue;
	volatile uint8_t ucNotifyState;
#endif

/* See the comments above the definition of
tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE. */
#if (tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0)
	uint8_t ucStaticallyAllocated; /*< Set to pdTRUE if the task is a statically allocated to ensure no attempt is
	                                  made to free the memory. */
#endif

#if (INCLUDE_xTaskAbortDelay == 1)
	uint8_t ucDelayAborted;
#endif

} tskTCB;

/* The old tskTCB name is maintained above then typedefed to the new TCB_t name
below to enable the use of older kernel aware debuggers. */
typedef tskTCB TCB_t;

extern TCB_t* volatile pxCurrentTCB;