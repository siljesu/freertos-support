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
#define ACTION_EVENT_BIT ( 1UL << 0UL ) // Bit for event group to check

TickType_t uxClockOffsetInTicks = 0;
EventGroupHandle_t xEventGroupHandle;
UBaseType_t uxSavedInterruptStatus;

int lf_critical_section_enter(){

    /* Check context to decide which API to use */
    if (xPortIsInsideInterrupt()){
        uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
    } else {
        taskENTER_CRITICAL();
    }
    return 0;
}

int lf_critical_section_exit(){

    /* Check context to decide which API to use */
    if (xPortIsInsideInterrupt()) {
        taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
    } else {
        taskEXIT_CRITICAL();
    }
    return 0;
}

int lf_notify_of_event(){
    BaseType_t xHigherPriorityTaskWoken, xResult;

    /* xHigherPriorityTaskWoken must be initialised to pdFALSE. */
    xHigherPriorityTaskWoken = pdFALSE;

    /* Set action bit in xEventGroupHandle. */
    xResult = xEventGroupSetBitsFromISR(xEventGroupHandle, ACTION_EVENT_BIT, &xHigherPriorityTaskWoken);

    /* Was the message posted successfully? */
    if( xResult != pdFAIL )
    {
        /* If xHigherPriorityTaskWoken is now set to pdTRUE then a context
        switch should be requested. */
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        return 0;
    } else {
        return -1;
    }
}

void lf_initialize_clock(void){

    xEventGroupHandle = xEventGroupCreate();

    /* Did event group creation fail? */
    if (xEventGroupHandle == NULL) {
        PRINTF("Insufficient memory to create Event Group");
        // Now what?
    }

    /* Get tick count at start of LF clock to offset time */
    uxClockOffsetInTicks = xTaskGetTickCount();
}

int lf_clock_gettime(instant_t* t){

    TickType_t uxTicks = xTaskGetTickCount() - uxClockOffsetInTicks;

    /* Transform from ticks to ns */
    *t = ((instant_t)TICKS_TO_NS(uxTicks, configTICK_RATE_HZ));
    return 0;
}

int lf_sleep(interval_t sleep_duration){
    EventBits_t uxResultBits;

    lf_critical_section_exit();

    /* Let current task wait for full sleep_duration, or be interrupted by action event */
    uxResultBits = xEventGroupWaitBits(xEventGroupHandle, ACTION_EVENT_BIT, pdTRUE, pdFALSE, pdMS_TO_TICKS(sleep_duration/1000000));

    lf_critical_section_enter();

    if ((uxResultBits & ACTION_EVENT_BIT) == 0) {
        /* Task waited for full sleep_duration */
        return 0;
    } else {
        /* Task wait was interrupted */
        return -1;
    }
}

int lf_sleep_until(instant_t wakeup_time){
    instant_t now;
    lf_clock_gettime(&now);
    
    interval_t sleep_duration = wakeup_time - now;
    
    return lf_sleep(sleep_duration);
}
