#include <stdlib.h> // Defines malloc.
#include <string.h> // Defines memcpy.
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#include "lf_freertos_support.h"
#include "../platform.h"
#include "../utils/util.h"

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
    taskENTER_CRITICAL();
}

/**
 * Exit the critical section entered with lf_lock_time().
 * @return 0 on success, platform-specific error number otherwise.
 */
int lf_critical_section_exit(){
    taskEXIT_CRITICAL();
}

/**
 * Notify any listeners that an event has been created.
 * The caller should call lf_critical_section_enter() before calling this function.
 * @return 0 on success, platform-specific error number otherwise.
 */
int lf_notify_of_event(){

}

/**
 * @brief Get the number of cores on the host machine.
 */
int lf_available_cores(){
    return 1; // freertos doesn't support multicores.
}

/**
 * Create a new thread, starting with execution of lf_thread
 * getting passed arguments. The new handle is stored in thread_id.
 *
 * @return 0 on success, platform-specific error number otherwise.
 *
 */
int lf_thread_create(lf_thread_t* thread, void *(*lf_thread) (void *), void* arguments){ //may yse xTaskCreateStatic if static allocation
    
    BaseType_t xReturned;
    xReturned = xTaskCreate(lf_thread, NULL, 100, arguments, 1, NULL); // No name, 100*4 Byte stack, priority 1, no handle
    if (xReturned == pdPASS){
        return 0;
    } else {
        return errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY; //what to return if error?
    }
}

/**
 * Make calling thread wait for termination of the thread.  The
 * exit status of the thread is stored in thread_return, if thread_return
 * is not NULL.
 * 
 * @return 0 on success, platform-specific error number otherwise.
 */
int lf_thread_join(lf_thread_t thread, void** thread_return){
    vTaskDelete(thread); //er dette en handle, da?
    return 0; //how to check if failed?
}

/**
 * Initialize a mutex.
 * 
 * @return 0 on success, platform-specific error number otherwise.
 */
int lf_mutex_init(lf_mutex_t* mutex){
    SemaphoreHandle_t xMutexHandle; // hvordan oversette dette til lf_mutex_t?
    xMutexHandle = xSemaphoreCreateMutex();
    if (xMutexHandle == NULL){
        return 1; //not success - should have something else here?
    } else {
        return 0;
    }
}

/**
 * Lock a mutex.
 * 
 * @return 0 on success, platform-specific error number otherwise.
 */
int lf_mutex_lock(lf_mutex_t* mutex){
    xSemaphoreTake( mutex, portMAX_DELAY ); //if not use portMAX_DELAY
}

/** 
 * Unlock a mutex.
 * 
 * @return 0 on success, platform-specific error number otherwise.
 */
int lf_mutex_unlock(lf_mutex_t* mutex){

}

/** 
 * Initialize a conditional variable.
 * 
 * @return 0 on success, platform-specific error number otherwise.
 */
int lf_cond_init(lf_cond_t* cond){

}

/** 
 * Wake up all threads waiting for condition variable cond.
 * 
 * @return 0 on success, platform-specific error number otherwise.
 */
int lf_cond_broadcast(lf_cond_t* cond){

}

/** 
 * Wake up one thread waiting for condition variable cond.
 * 
 * @return 0 on success, platform-specific error number otherwise.
 */
int lf_cond_signal(lf_cond_t* cond){

}

/** 
 * Wait for condition variable "cond" to be signaled or broadcast.
 * "mutex" is assumed to be locked before.
 * 
 * @return 0 on success, platform-specific error number otherwise.
 */
int lf_cond_wait(lf_cond_t* cond, lf_mutex_t* mutex){

}

/** 
 * Block current thread on the condition variable until condition variable
 * pointed by "cond" is signaled or time pointed by "absolute_time_ns" in
 * nanoseconds is reached.
 * 
 * @return 0 on success, LF_TIMEOUT on timeout, and platform-specific error
 *  number otherwise.
 */
int lf_cond_timedwait(lf_cond_t* cond, lf_mutex_t* mutex, instant_t absolute_time_ns){

}


/**
 * Initialize the LF clock. Must be called before using other clock-related APIs.
 */
void lf_initialize_clock(void){

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

}

/**
 * Pause execution for a given number duration.
 * @return 0 if sleep was completed, or -1 if it was interrupted.
 */
int lf_sleep(interval_t sleep_duration){

}

int lf_sleep_until(instant_t wakeup_time){

}
