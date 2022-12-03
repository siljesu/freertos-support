#include <stdlib.h> // Defines malloc.
#include <string.h> // Defines memcpy.
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "event_groups.h"

#include "lf_freertos_support.h"
#include "../platform.h"
#include "../utils/util.h"

#define TICKS_TO_NS(ticks, freq) ((uint64_t)ticks*1000000000)/freq
#define NS_TO_TICKS(ns, freq) (uint32_t)((ns*freq)/1000000000)
#define ACTION_EVENT_BIT ( 1UL << 0UL ) // Bit for event group to check

uint32_t clock_offset_ticks = 0; 
EventGroupHandle_t eventGroupHandle;
UBaseType_t uxSavedInterruptStatus;

// configUSE_16_BIT_TICKS = 0; For å få uint32_t TickType_t
// configSUPPORT_DYNAMIC_ALLOCATION must be set to 1 for tassk creation
// INCLUDE_vTaskDelete must be defined as 1 for task delete function to be available.
// configUSE_MUTEXES must be set to 1 in FreeRTOSConfig.h for mutexes to be available.

/**
 * Enter a critical section where logical time and the event queue are guaranteed
 * to not change unless they are changed within the critical section.
 * In platforms with threading support, this normally will be implemented
 * by acquiring a mutex lock. In platforms without threading support,
 * this can be implemented by disabling interrupts.
 * Users of this function must ensure that lf_init_critical_sections() is
 * called first and that lf_critical_section_exit() is called later.
 * @return 0 on success, platform-specific error number otherwise.
 */
int lf_critical_section_enter(){
    uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
    return 0;
}

/**
 * Exit the critical section entered with lf_lock_time().
 * @return 0 on success, platform-specific error number otherwise.
 */
int lf_critical_section_exit(){
    taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
    return 0;
}

/**
 * Notify any listeners that an event has been created.
 * The caller should call lf_critical_section_enter() before calling this function.
 * @return 0 on success, platform-specific error number otherwise.
 */
int lf_notify_of_event(){
    BaseType_t xHigherPriorityTaskWoken, xResult;

    /* xHigherPriorityTaskWoken must be initialised to pdFALSE. */
    xHigherPriorityTaskWoken = pdFALSE;

    /* Set bit 0 and bit 4 in xEventGroup. */
    xResult = xEventGroupSetBitsFromISR(
                                eventGroupHandle,   /* The event group being updated. */
                                ACTION_EVENT_BIT, /* The bits being set. */
                                &xHigherPriorityTaskWoken );
    /* Was the message posted successfully? */
    if( xResult != pdFAIL )
    {
        /* If xHigherPriorityTaskWoken is now set to pdTRUE then a context
        switch should be requested.  The macro used is port specific and will
        be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
        the documentation page for the port being used. */
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
    return 0;
}

/**
 * Initialize the LF clock. Must be called before using other clock-related APIs.
 */
void lf_initialize_clock(void){

    eventGroupHandle = xEventGroupCreate();
    if (eventGroupHandle == NULL) {
        PRINTF("Insufficient memory to create Event Group");
    }

    /* Get tick count at start of LF clock to offset time */
    clock_offset_ticks = xTaskGetTickCount();
}

/**
 * Fetch the value of an internal (and platform-specific) physical clock and 
 * store it in `t`.
 * 
 * Ideally, the underlying platform clock should be monotonic. However, the
 * core lib tries to enforce monotonicity at higher level APIs (see tag.h).
 * 
 * @return 0 for success, or -1 for failure
 */
int lf_clock_gettime(instant_t* t){
    TickType_t ticks = xTaskGetTickCount() - clock_offset_ticks;
    *t = ((instant_t)TICKS_TO_NS(ticks, configTICK_RATE_HZ));
    return 0;
}

/**
 * Pause execution for a given number duration.
 * @return 0 if sleep was completed, or -1 if it was interrupted.
 */
int lf_sleep(interval_t sleep_duration){

    // Event group blocks for sleep_duration, unless action event happens
    // What if something else happens?
    PRINTF("\r\n going to sleep for %lld ns\r\n", sleep_duration);
    PRINTF("\r\n which equals %ld ticks (freq: %ld, sysclock: %ld)\r\n", NS_TO_TICKS(sleep_duration,configTICK_RATE_HZ), configTICK_RATE_HZ, configCPU_CLOCK_HZ);
    EventBits_t resultBits;

    lf_critical_section_exit();

    TickType_t ticks = xTaskGetTickCount() - clock_offset_ticks;
    PRINTF("\r\n Ticks before sleep: %ld \r\n", ticks);

    resultBits = xEventGroupWaitBits( eventGroupHandle, ACTION_EVENT_BIT, pdTRUE, pdFALSE, pdMS_TO_TICKS(sleep_duration/1000000) );

    ticks = xTaskGetTickCount() - clock_offset_ticks;
    PRINTF("\r\n Ticks after sleep: %ld \r\n", ticks);

    lf_critical_section_enter();

    if ((resultBits & ACTION_EVENT_BIT) == 0) {
        // blocked for sleep_duration
        PRINTF("\r\n Sleep success \r\n");
        return 0;
    } else {
        // was interrupted by action
        PRINTF("\r\n Sleep interrupted \r\n");
        return -1;
    }
}

int lf_sleep_until(instant_t wakeup_time){
    
    instant_t now;
    lf_clock_gettime(&now);
    interval_t sleep_duration = wakeup_time - now;
    
    return lf_sleep(sleep_duration);
}
