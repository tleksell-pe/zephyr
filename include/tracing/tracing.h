/*
 * Copyright (c) 2018 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ZEPHYR_INCLUDE_TRACING_TRACING_H_
#define ZEPHYR_INCLUDE_TRACING_TRACING_H_

#include <kernel.h>

/* Below IDs are used with systemview, not final to the zephyr tracing API */
#define SYS_TRACE_ID_OFFSET                  (32u)

#define SYS_TRACE_ID_MUTEX_INIT              (1u + SYS_TRACE_ID_OFFSET)
#define SYS_TRACE_ID_MUTEX_UNLOCK            (2u + SYS_TRACE_ID_OFFSET)
#define SYS_TRACE_ID_MUTEX_LOCK              (3u + SYS_TRACE_ID_OFFSET)
#define SYS_TRACE_ID_SEMA_INIT               (4u + SYS_TRACE_ID_OFFSET)
#define SYS_TRACE_ID_SEMA_GIVE               (5u + SYS_TRACE_ID_OFFSET)
#define SYS_TRACE_ID_SEMA_TAKE               (6u + SYS_TRACE_ID_OFFSET)
#define SYS_TRACE_ID_SLEEP                   (7u + SYS_TRACE_ID_OFFSET)

#ifdef CONFIG_PERCEPIO_TRACERECORDER
#include "tracing_tracerecorder.h"

#elif defined CONFIG_SEGGER_SYSTEMVIEW
#include "tracing_sysview.h"

#elif defined CONFIG_TRACING_CPU_STATS
#include "tracing_cpu_stats.h"

#elif defined CONFIG_TRACING_CTF
#include "tracing_ctf.h"

#elif defined CONFIG_TRACING_TEST
#include "tracing_test.h"

#else

/**
 * @brief Tracing APIs
 * @defgroup tracing_apis Tracing APIs
 * @{
 */
/**
 * @brief Called before a thread has been selected to run
 *
 * @note Deprecated: This call has been migrated into the new tracing system.
 */
#define sys_trace_thread_switched_out()

/**
 * @brief Called after a thread has been selected to run
 *
 * @note Deprecated: This call has been migrated into the new tracing system.
 */
#define sys_trace_thread_switched_in()

/**
 * @brief Called when setting priority of a thread
 * @param thread Thread structure
 *
 * @note Deprecated: This call has been migrated into the new tracing system.
 */
#define sys_trace_thread_priority_set(thread)

/**
 * @brief Called when a thread is being created
 * @param thread Thread structure
 *
 * @note Deprecated: This call has been migrated into the new tracing system.
 */
#define sys_trace_thread_create(thread)

/**
 * @brief Called when a thread is being aborted
 * @param thread Thread structure
 *
 * @note Deprecated: This call has been migrated into the new tracing system.
 */
#define sys_trace_thread_abort(thread)

/**
 * @brief
 * @param thread Thread structure
 *
 * @note This call can be kept as is and migrated into the thread
 * section when updating the tracing system with the new changes.
 */
#define sys_trace_thread_suspend(thread)

/**
 * @brief Called when a thread is being resumed from suspension
 * @param thread Thread structure
 *
 * @note This call can be kept as is and migrated into the thread
 * section when updating the tracing system with the new changes.
 */
#define sys_trace_thread_resume(thread)

/**
 * @brief Called when a thread is ready to run
 * @param thread Thread structure
 *
 * @note Deprecated: This call has been migrated into the new tracing system.
 */
#define sys_trace_thread_ready(thread)

/**
 * @brief Called when a thread is pending
 * @param thread Thread structure
 *
 * @note Deprecated: This call has been migrated into the new tracing system.
 */
#define sys_trace_thread_pend(thread)

/**
 * @brief Provide information about specific thread
 * @param thread Thread structure
 *
 * @note This call can be kept as is and migrated into the thread
 * section when updating the tracing system with the new changes.
 */
#define sys_trace_thread_info(thread)

/**
 * @brief Called when a thread name is set
 * @param thread Thread structure
 *
 * @note Deprecated: This call has been migrated into the new tracing system.
 */
#define sys_trace_thread_name_set(thread)

/**
 * @brief Called when entering an ISR
 *
 * @note This call can be kept for the new tracing system but it would
 * be great if we could get the ISR parameters in this call (cb func) as
 * unique identifiers for use in tracing.
 */
#define sys_trace_isr_enter()

/**
 * @brief Called when exiting an ISR
 *
 * @note This call can be kept for the new tracing system but it would
 * be great if we could get the ISR parameters in this call (cb func) as
 * unique identifiers for use in tracing.
 */
#define sys_trace_isr_exit()

/**
 * @brief Called when exiting an ISR and switching to scheduler
 *
 * @note This call can be kept for the new tracing system but it would
 * be great if we could get the ISR parameters in this call (cb func) as
 * unique identifiers for use in tracing.
 */
#define sys_trace_isr_exit_to_scheduler()

/**
 * @brief Can be called with any id signifying a new call
 * @param id ID of the operation that was started
 *
 * @note Deprecated: This call has been migrated into the new tracing system.
 */
#define sys_trace_void(id)

/**
 * @brief Can be called with any id signifying ending a call
 * @param id ID of the operation that was completed
 *
 * @note Deprecated: This call has been migrated into the new tracing system.
 */
#define sys_trace_end_call(id)

/**
 * @brief Called when the cpu enters the idle state
 *
 * @note This call can be kept for the new tracing system.
 */
#define sys_trace_idle()

/**
 * @brief Trace initialisation of a Semaphore
 * @param sem Semaphore object
 *
 * @note Deprecated: This call has been migrated into the new tracing system.
 */
#define sys_trace_semaphore_init(sem)

/**
 * @brief Trace taking a Semaphore
 * @param sem Semaphore object
 *
 * @note Deprecated: This call has been migrated into the new tracing system.
 */
#define sys_trace_semaphore_take(sem)

/**
 * @brief Trace giving a Semaphore
 * @param sem Semaphore object
 *
 * @note Deprecated: This call has been migrated into the new tracing system.
 */
#define sys_trace_semaphore_give(sem)

/**
 * @brief Trace initialisation of a Mutex
 * @param mutex  Mutex object
 *
 * @note Deprecated: This call has been migrated into the new tracing system.
 */
#define sys_trace_mutex_init(mutex)

/**
 * @brief Trace locking a Mutex
 * @param mutex Mutex object
 *
 * @note Deprecated: This call has been migrated into the new tracing system.
 */
#define sys_trace_mutex_lock(mutex)

/**
 * @brief Trace unlocking a Mutex
 * @param mutex Mutex object
 *
 * @note Deprecated: This call has been migrated into the new tracing system.
 */
#define sys_trace_mutex_unlock(mutex)
/**
 * @}
 */




/**
 * @note The following code contains suggestions for new trace macros that replace and extend
 * the old system in favor of detailed tracing for all kernel objects. The code above contains the
 * old macros which has been left in place for now to avoid breaking the other parts of the tracing
 * system (sysview etc.).
 *
 * To avoid naming conflicts the new tracing macros are currently marked with a "port" infix which
 * should be removed if this system is adopted.
 */


/**
 * @brief Tracing APIs
 * @defgroup tracing_apis Tracing APIs
 * @{
 */


/**
 * @brief Syscall Tracing APIs
 * @defgroup syscall_tracing_apis Syscall Tracing APIs
 * @ingroup tracing_apis
 * @{
 */

/**
 * @brief Called when entering a Syscall check
 */
#define sys_port_trace_syscall_enter()

/**
 * @brief Called when exiting a Syscall check
 */
#define sys_port_trace_syscall_exit()

/**
 * @}
 */ /* end of syscall_tracing_apis */




/**
 * @brief Thread Tracing APIs
 * @defgroup thread_tracing_apis Thread Tracing APIs
 * @ingroup tracing_apis
 * @{
 */

/**
 * @brief Called when entering a k_thread_foreach call
 */
#define sys_port_trace_k_thread_foreach_enter()

/**
 * @brief Called when exiting a k_thread_foreach call
 */
#define sys_port_trace_k_thread_foreach_exit()

/**
 * @brief Called when entering a k_thread_foreach_unlocked
 */
#define sys_port_trace_k_thread_foreach_unlocked_enter()

/**
 * @brief Called when exiting a k_thread_foreach_unlocked
 */
#define sys_port_trace_k_thread_foreach_unlocked_exit()

/**
 * @brief Trace creating a Thread
 * @param new_thread Thread object
 */
#define sys_port_trace_k_thread_create(new_thread)

/**
 * @brief Trace Thread entering user mode
 */
#define sys_port_trace_k_thread_user_mode_enter()

/**
 * @brief Called when entering a k_thread_join
 * @param thread Thread object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_thread_join_enter(thread, timeout)

/**
 * @brief Called when k_thread_join blocks
 * @param thread Thread object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_thread_join_blocking(thread, timeout)

/**
 * @brief Called when exiting k_thread_join
 * @param thread Thread object
 * @param timeout Timeout period
 * @param ret Return value
 */
#define sys_port_trace_k_thread_join_exit(thread, timeout, ret)

/**
 * @brief Called when entering k_thread_sleep
 * @param timeout Timeout period
 */
#define sys_port_trace_k_thread_sleep_enter(timeout)

/**
 * @brief Called when exiting k_thread_sleep
 * @param timeout Timeout period
 * @param ret Return value
 */
#define sys_port_trace_k_thread_sleep_exit(timeout, ret)

/**
 * @brief Called when entering k_thread_msleep
 * @param ms Duration in milliseconds
 */
#define sys_port_trace_k_thread_msleep_enter(ms)

/**
 * @brief Called when exiting k_thread_msleep
 * @param ms Duration in milliseconds
 * @param ret Return value
 */
#define sys_port_trace_k_thread_msleep_exit(ms, ret)

/**
 * @brief Called when entering k_thread_usleep
 * @param us Duration in microseconds
 */
#define sys_port_trace_k_thread_usleep_enter(us)

/**
 * @brief Called when exiting k_thread_usleep
 * @param us Duration in microseconds
 * @param ret Return value
 */
#define sys_port_trace_k_thread_usleep_exit(us, ret)

/**
 * @brief Called when entering k_thread_busy_wait
 * @param usec_to_wait Duration in microseconds
 */
#define sys_port_trace_k_thread_busy_wait_enter(usec_to_wait)

/**
 * @brief Called when exiting k_thread_busy_wait
 * @param usec_to_wait Duration in microseconds
 */
#define sys_port_trace_k_thread_busy_wait_exit(usec_to_wait)

/**
 * @brief Called when a thread yields
 */
#define sys_port_trace_k_thread_yield()

/**
 * @brief Called when a thread wakes up
 * @param thread Thread object
 */
#define sys_port_trace_k_thread_wakeup(thread)

/**
 * @brief Called when a thread is started
 * @param thread Thread object
 */
#define sys_port_trace_k_thread_start(thread)

/**
 * @brief Called when a thread is being aborted
 * @param thread Thread object
 */
#define sys_port_trace_k_thread_abort(thread)

/**
 * @brief Called when setting priority of a thread
 * @param thread Thread object
 */
#define sys_port_trace_k_thread_priority_set(thread)

/**
 * @brief Called when a thread is being suspended
 * @param thread Thread object
 */
#define sys_port_trace_k_thread_suspend(thread)

/**
 * @brief Called when a thread is being resumed from suspension
 * @param thread Thread object
 */
#define sys_port_trace_k_thread_resume(thread)

/**
 * @brief Called when the thread scheduler is locked
 */
#define sys_port_trace_k_thread_sched_lock()

/**
 * @brief Called when the thread sceduler is unlocked
 */
#define sys_port_trace_k_thread_sched_unlock()

/**
 * @brief Called when a thread name is set
 * @param thread Thread object
 * @param ret Return value
 */
#define sys_port_trace_k_thread_name_set(thread, ret)

/**
 * @brief Called before a thread has been selected to run
 */
#define sys_port_trace_k_thread_switched_out()

/**
 * @brief Called after a thread has been selected to run
 */
#define sys_port_trace_k_thread_switched_in()

/**
 * @brief Called when a thread is ready to run
 * @param thread Thread object
 */
#define sys_port_trace_k_thread_ready(thread)

/**
 * @brief Called when a thread is pending
 * @param thread Thread object
 */
#define sys_port_trace_k_thread_pend(thread)

/**
 * @brief Provide information about specific thread
 * @param thread Thread object
 */
#define sys_port_trace_k_thread_info(thread)

/**
 * @}
 */ /* end of thread_tracing_apis */




/**
 * @brief Semaphore Tracing APIs
 * @defgroup sem_tracing_apis Semaphore Tracing APIs
 * @ingroup tracing_apis
 * @{
 */

/**
 * @brief Trace initialisation of a Semaphore
 * @param sem Semaphore object
 * @param ret Return value
 */
#define sys_port_trace_k_sem_init(sem, ret)

/**
 * @brief Trace giving a Semaphore entry
 * @param sem Semaphore object
 */
#define sys_port_trace_k_sem_give_enter(sem)

/**
 * @brief Trace giving a Semaphore exit
 * @param sem Semaphore object
 */
#define sys_port_trace_k_sem_give_exit(sem)

/**
 * @brief Trace taking a Semaphore attempt start
 * @param sem Semaphore object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_sem_take_enter(sem, timeout)

/**
 * @brief Trace taking a Semaphore attempt blocking
 * @param sem Semaphore object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_sem_take_blocking(sem, timeout)

/**
 * @brief Trace taking a Semaphore attempt outcome
 * @param sem Semaphore object
 * @param timeout Timeout period
 * @param ret Return value
 */
#define sys_port_trace_k_sem_take_exit(sem, timeout, ret)

/**
 * @brief Trace resetting a Semaphore
 * @param sem Semaphore object
 */
#define sys_port_trace_k_sem_reset(sem)

/**
 * @}
 */ /* end of sem_tracing_apis */




/**
 * @brief Mutex Tracing APIs
 * @defgroup mutex_tracing_apis Mutex Tracing APIs
 * @ingroup tracing_apis
 * @{
 */

/**
 * @brief Trace initialization of Mutex
 * @param mutex Mutex object
 * @param ret Return value
 */
#define sys_port_trace_k_mutex_init(mutex, ret)

/**
 * @brief Trace Mutex lock attempt start
 * @param mutex Mutex object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_mutex_lock_enter(mutex, timeout)

/**
 * @brief Trace Mutex lock attempt blocking
 * @param mutex Mutex object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_mutex_lock_blocking(mutex, timeout)

/**
 * @brief Trace Mutex lock attempt outcome
 * @param mutex Mutex object
 * @param timeout Timeout period
 * @param ret Return value
 */
#define sys_port_trace_k_mutex_lock_exit(mutex, timeout, ret)

/**
 * @brief Trace Mutex unlock entry
 * @param mutex Mutex object
 */
#define sys_port_trace_k_mutex_unlock_enter(mutex)

/**
 * @brief Trace Mutex unlock exit
 */
#define sys_port_trace_k_mutex_unlock_exit(mutex, ret)

/**
 * @}
 */ /* end of mutex_tracing_apis */




/**
 * @brief Conditional Variable Tracing APIs
 * @defgroup condvar_tracing_apis Conditional Variable Tracing APIs
 * @ingroup tracing_apis
 * @{
 */

/**
 * @brief Trace initialization of Conditional Variable
 * @param condvar Conditional Variable object
 * @param ret Return value
 */
#define sys_port_trace_k_condvar_init(condvar, ret)

/**
 * @brief Trace Conditional Variable signaling start
 * @param condvar Conditional Variable object
 * @param ret Return value
 */
#define sys_port_trace_k_condvar_signal_enter(condvar)

/**
 * @brief Trace Conditional Variable signaling blocking
 * @param condvar Conditional Variable object
 * @param timeout Timeout period
 * @param ret Return value
 */
#define sys_port_trace_k_condvar_signal_blocking(condvar, timeout)

/**
 * @brief Trace Conditional Variable signaling outcome
 * @param condvar Conditional Variable object
 * @param ret Return value
 */
#define sys_port_trace_k_condvar_signal_exit(condvar, ret)

/**
 * @brief Trace Conditional Variable broadcast enter
 * @param condvar Conditional Variable object
 */
#define sys_port_trace_k_condvar_broadcast_enter(condvar)

/**
 * @brief Trace Conditional Variable broadcast exit
 * @param condvar Conditional Variable object
 * @param ret Return value
 */
#define sys_port_trace_k_condvar_broadcast_exit(condvar, ret)

/**
 * @brief Trace Conditional Variable wait enter
 * @param condvar Conditional Variable object
 */
#define sys_port_trace_k_condvar_wait_enter(condvar)

/**
 * @brief Trace Conditional Variable wait exit
 * @param condvar Conditional Variable object
 * @param ret Return value
 */
#define sys_port_trace_k_condvar_wait_exit(condvar, ret)

/**
 * @}
 */ /* end of condvar_tracing_apis */




/**
 * @brief Queue Tracing APIs
 * @defgroup queue_tracing_apis Queue Tracing APIs
 * @ingroup tracing_apis
 * @{
 */

/**
 * @brief Trace initialization of Queue
 * @param queue Queue object
 */
#define sys_port_trace_k_queue_init(queue)

/**
 * @brief Trace Queue cancel wait
 * @param queue Queue object
 */
#define sys_port_trace_k_queue_cancel_wait(queue)

/**
 * @brief Trace Queue insert attempt entry
 * @param queue Queue object
 * @param alloc Allocation flag
 * @param timeout Timeout period
 */
#define sys_port_trace_k_queue_queue_insert_enter(queue, alloc)

/**
 * @brief Trace Queue insert attempt blocking
 * @param queue Queue object
 * @param alloc Allocation flag
 * @param timeout Timeout period
 */
#define sys_port_trace_k_queue_queue_insert_blocking(queue, alloc, timeout)

/**
 * @brief Trace Queue insert attempt outcome
 * @param queue Queue object
 * @param alloc Allocation flag
 * @param ret Return value
 */
#define sys_port_trace_k_queue_queue_insert_exit(queue, alloc, ret)

/**
 * @brief Trace Queue append enter
 * @param queue Queue object
 */
#define sys_port_trace_k_queue_append_enter(queue)

/**
 * @brief Trace Queue append exit
 * @param queue Queue object
 */
#define sys_port_trace_k_queue_append_exit(queue)

/**
 * @brief Trace Queue alloc append enter
 * @param queue Queue object
 */
#define sys_port_trace_k_queue_alloc_append_enter(queue)

/**
 * @brief Trace Queue alloc append exit
 * @param queue Queue object
 * @param ret Return value
 */
#define sys_port_trace_k_queue_alloc_append_exit(queue, ret)

/**
 * @brief Trace Queue prepend enter
 * @param queue Queue object
 */
#define sys_port_trace_k_queue_prepend_enter(queue)

/**
 * @brief Trace Queue prepend exit
 * @param queue Queue object
 */
#define sys_port_trace_k_queue_prepend_exit(queue)

/**
 * @brief Trace Queue alloc prepend enter
 * @param queue Queue object
 */
#define sys_port_trace_k_queue_alloc_prepend_enter(queue)

/**
 * @brief Trace Queue alloc prepend exit
 * @param queue Queue object
 * @param ret Return value
 */
#define sys_port_trace_k_queue_alloc_prepend_exit(queue, ret)

/**
 * @brief Trace Queue insert attempt entry
 * @param queue Queue object
 */
#define sys_port_trace_k_queue_insert_enter(queue)

/**
 * @brief Trace Queue insert attempt blocking
 * @param queue Queue object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_queue_insert_blocking(queue, timeout)

/**
 * @brief Trace Queue insert attempt exit
 * @param queue Queue object
 */
#define sys_port_trace_k_queue_insert_exit(queue)

/**
 * @brief Trace Queue append list enter
 * @param queue Queue object
 */
#define sys_port_trace_k_queue_append_list_enter(queue)

/**
 * @brief Trace Queue append list exit
 * @param queue Queue object
 * @param ret Return value
 */
#define sys_port_trace_k_queue_append_list_exit(queue, ret)

/**
 * @brief Trace Queue merge slist enter
 * @param queue Queue object
 */
#define sys_port_trace_k_queue_merge_slist_enter(queue)

/**
 * @brief Trace Queue merge slist exit
 * @param queue Queue object
 * @param ret Return value
 */
#define sys_port_trace_k_queue_merge_slist_exit(queue, ret)

/**
 * @brief Trace Queue get attempt enter
 * @param queue Queue object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_queue_get_enter(queue, timeout)

/**
 * @brief Trace Queue get attempt blockings
 * @param queue Queue object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_queue_get_blocking(queue, timeout)

/**
 * @brief Trace Queue get attempt outcome
 * @param queue Queue object
 * @param timeout Timeout period
 * @param ret Return value
 */
#define sys_port_trace_k_queue_get_exit(queue, timeout, ret)

/**
 * @brief Trace Queue remove enter
 * @param queue Queue object
 */
#define sys_port_trace_k_queue_remove_enter(queue)

/**
 * @brief Trace Queue remove exit
 * @param queue Queue object
 * @param ret Return value
 */
#define sys_port_trace_queue_remove_exit(queue, ret)

/**
 * @brief Trace Queue unique append enter
 * @param queue Queue object
 */
#define sys_port_trace_k_queue_unique_append_enter(queue)

/**
 * @brief Trace Queue unique append exit
 * @param queue Queue object
 *
 * @param ret Return value
 */
#define sys_port_trace_k_queue_unique_append_exit(queue, ret)

/**
 * @brief Trace Queue peek head
 * @param queue Queue object
 * @param ret Return value
 */
#define sys_port_trace_k_queue_peek_head(queue, ret)

/**
 * @brief Trace Queue peek tail
 * @param queue Queue object
 * @param ret Return value
 */
#define sys_port_trace_k_queue_peek_tail(queue, ret)

/**
 * @}
 */ /* end of queue_tracing_apis */




/**
 * @note Tracing hooks for FIFO are not yet implemented pending necessary changes
 * which plays nice with system calls and user mode.
 *
 * @brief FIFO Tracing APIs
 * @defgroup fifo_tracing_apis FIFO Tracing APIs
 * @ingroup tracing_apis
 * @{
 */

/**
 * @brief Trace initialization of FIFO
 * @param fifo FIFO object
 */
#define sys_port_trace_k_fifo_init(fifo)

/**
 * @brief Trace FIFO cancel wait
 * @param fifo FIFO object
 */
#define sys_port_trace_k_fifo_cancel_wait(fifo)

/**
 * @brief Trace FIFO put
 * @param fifo FIFO object
 */
#define sys_port_trace_k_fifo_put(fifo)

/**
 * @brief Trace FIFO alloc put entry
 * @param fifo FIFO object
 */
#define sys_port_trace_k_fifo_alloc_put_enter(fifo)

/**
 * @brief Trace FIFO alloc put exit
 * @param fifo FIFO object
 * @param ret Return value
 */
#define sys_port_trace_k_fifo_alloc_put_exit(fifo, ret)

/**
 * @brief Trace FIFO put list
 * @param fifo FIFO object
 */
#define sys_port_trace_k_fifo_put_list(fifo)

/**
 * @brief Trace FIFO put slist
 * @param fifo FIFO object
 */
#define sys_port_trace_k_fifo_put_slist(fifo)

/**
 * @brief Trace FIFO get attempt start
 * @param fifo FIFO object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_fifo_get_enter(fifo, timeout)

/**
 * @brief Trace FIFO get attempt outcome
 * @param fifo FIFO object
 * @param timeout Timeout period
 * @param ret Return value
 */
#define sys_port_trace_k_fifo_get_exit(fifo, timeout, ret)

/**
 * @brief Trace FIFO peek head
 * @param fifo FIFO object
 * @param ret Return value
 */
#define sys_port_trace_k_fifo_peek_head(fifo, ret)

/**
 * @brief Trace FIFO peek tail
 * @param fifo FIFO object
 * @param ret Return value
 */
#define sys_port_trace_k_fifo_peek_tail(fifo, ret)

/**
 * @}
 */ /* end of fifo_tracing_apis */




/**
 * @note Tracing hooks for LIFO are not yet implemented pending necessary changes
 * which plays nice with system calls and user mode.
 *
 * @brief LIFO Tracing APIs
 * @defgroup lifo_tracing_apis Mutex Tracing APIs
 * @ingroup tracing_apis
 * @{
 */

/**
 * @brief Trace initialization of LIFO
 * @param fifo LIFO object
 */
#define sys_port_trace_k_lifo_init(lifo)

/**
 * @brief Trace LIFO put
 * @param lifo LIFO object
 */
#define sys_port_trace_k_lifo_put(lifo)

/**
 * @brief Trace LIFO alloc put entry
 * @param lifo LIFO object
 */
#define sys_port_trace_k_lifo_alloc_put_enter(lifo)

/**
 * @brief Trace LIFO alloc put exit
 * @param lifo LIFO object
 * @param ret Return value
 */
#define sys_port_trace_k_lifo_alloc_put_exit(lifo, ret)

/**
 * @brief Trace LIFO get attempt start
 * @param lifo LIFO object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_lifo_get_enter(lifo, timeout)

/**
 * @brief Trace LIFO get attempt outcome
 * @param lifo LIFO object
 * @param timeout Timeout period
 * @param ret Return value
 */
#define sys_port_trace_k_lifo_get_exit(lifo, timeout, ret)

/**
 * @}
 */ /* end of lifo_tracing_apis */




/**
 * @brief Stack Tracing APIs
 * @defgroup stack_tracing_apis Stack Tracing APIs
 * @ingroup tracing_apis
 * @{
 */

/**
 * @brief Trace initialization of Stack
 * @param stack Stack object
 */
#define sys_port_trace_k_stack_init(stack)

/**
 * @brief Trace Stack alloc init attempt entry
 * @param stack Stack object
 */
#define sys_port_trace_k_stack_alloc_init_enter(stack)

/**
 * @brief Trace Stack alloc init outcome
 * @param stack Stack object
 * @param ret Return value
 */
#define sys_port_trace_k_stack_alloc_init_exit(stack, ret)

/**
 * @brief Trace Stack cleanup attempt entry
 * @param stack Stack object
 */
#define sys_port_trace_k_stack_cleanup_enter(stack)

/**
 * @brief Trace Stack cleanup outcome
 * @param stack Stack object
 * @param ret Return value
 */
#define sys_port_trace_k_stack_cleanup_exit(stack, ret)

/**
 * @brief Trace Stack push attempt entry
 * @param stack Stack object
 */
#define sys_port_trace_k_stack_push_enter(stack)

/**
 * @brief Trace Stack push attempt outcome
 * @param stack Stack object
 * @param ret Return value
 */
#define sys_port_trace_k_stack_push_exit(stack, ret)

/**
 * @brief Trace Stack pop attempt entry
 * @param stack Stack object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_stack_pop_enter(stack, timeout)

/**
 * @brief Trace Stack pop attempt blocking
 * @param stack Stack object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_stack_pop_blocking(stack, timeout)

/**
 * @brief Trace Stack pop attempt outcome
 * @param stack Stack object
 * @param timeout Timeout period
 * @param ret Return value
 */
#define sys_port_trace_k_stack_pop_exit(stack, timeout, ret)

/**
 * @}
 */ /* end of stack_tracing_apis */




/**
 * @brief Message Queue Tracing APIs
 * @defgroup msgq_tracing_apis Message Queue Tracing APIs
 * @ingroup tracing_apis
 * @{
 */

/**
 * @brief Trace initialization of Message Queue
 * @param msgq Message Queue object
 */
#define sys_port_trace_k_msgq_init(msgq)

/**
 * @brief Trace Message Queue alloc init attempt entry
 * @param msgq Message Queue object
 */
#define sys_port_trace_k_msgq_alloc_init_enter(msgq)

/**
 * @brief Trace Message Queue alloc init attempt outcome
 * @param msgq Message Queue object
 * @param ret Return value
 */
#define sys_port_trace_k_msgq_alloc_init_exit(msgq, ret)

/**
 * @brief Trace Message Queue cleanup attempt entry
 * @param msgq Message Queue object
 */
#define sys_port_trace_k_msgq_cleanup_enter(msgq)

/**
 * @brief Trace Message Queue cleanup attempt outcome
 * @param msgq Message Queue object
 * @param ret Return value
 */
#define sys_port_trace_k_msgq_cleanup_exit(msgq, ret)

/**
 * @brief Trace Message Queue put attempt entry
 * @param msgq Message Queue object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_msgq_put_enter(msgq, timeout)

/**
 * @brief Trace Message Queue put attempt blocking
 * @param msgq Message Queue object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_msgq_put_blocking(msgq, timeout)

/**
 * @brief Trace Message Queue put attempt outcome
 * @param msgq Message Queue object
 * @param timeout Timeout period
 * @param ret Return value
 */
#define sys_port_trace_k_msgq_put_exit(msgq, timeout, ret)

/**
 * @brief Trace Message Queue get attempt entry
 * @param msgq Message Queue object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_msgq_get_enter(msgq, timeout)

/**
 * @brief Trace Message Queue get attempt blockings
 * @param msgq Message Queue object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_msgq_get_blocking(msgq, timeout)

/**
 * @brief Trace Message Queue get attempt outcome
 * @param msgq Message Queue object
 * @param timeout Timeout period
 * @param ret Return value
 */
#define sys_port_trace_k_msgq_get_exit(msgq, timeout, ret)

/**
 * @brief Trace Message Queue peek
 * @param msgq Message Queue object
 * @param ret Return value
 */
#define sys_port_trace_k_msgq_peek(msgq, ret)

/**
 * @brief Trace Message Queue purge
 * @param msgq Message Queue object
 */
#define sys_port_trace_k_msgq_purge(msgq)

/**
 * @}
 */ /* end of msgq_tracing_apis */




/**
 * @brief Mailbox Tracing APIs
 * @defgroup mbox_tracing_apis Mailbox Tracing APIs
 * @ingroup tracing_apis
 * @{
 */

/**
 * @brief Trace initialization of Mailbox
 * @param mbox Mailbox object
 */
#define sys_port_trace_k_mbox_init(mbox)

/**
 * @brief Trace Mailbox message put attempt entry
 * @param mbox Mailbox object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_mbox_message_put_enter(mbox, timeout)

/**
 * @brief Trace Mailbox message put attempt blocking
 * @param mbox Mailbox object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_mbox_message_put_blocking(mbox, timeout)

/**
 * @brief Trace Mailbox message put attempt outcome
 * @param mbox Mailbox object
 * @param timeout Timeout period
 * @param ret Return value
 */
#define sys_port_trace_k_mbox_message_put_exit(mbox, timeout, ret)

/**
 * @brief Trace Mailbox put attempt entry
 * @param mbox Mailbox object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_mbox_put_enter(mbox, timeout)

/**
 * @brief Trace Mailbox put attempt blocking
 * @param mbox Mailbox object
 * @param timeout Timeout period
 * @param ret Return value
 */
#define sys_port_trace_k_mbox_put_exit(mbox, timeout, ret)

/**
 * @brief Trace Mailbox async put entry
 * @param mbox Mailbox object
 * @param sem Semaphore object
 */
#define sys_port_trace_k_mbox_async_put_enter(mbox, sem)

/**
 * @brief Trace Mailbox async put exit
 * @param mbox Mailbox object
 * @param sem Semaphore object
 */
#define sys_port_trace_k_mbox_async_put_exit(mbox, sem)

/**
 * @brief Trace Mailbox get attempt entry
 * @param mbox Mailbox entry
 * @param timeout Timeout period
 */
#define sys_port_trace_k_mbox_get_enter(mbox, timeout)

/**
 * @brief Trace Mailbox get attempt blocking
 * @param mbox Mailbox entry
 * @param timeout Timeout period
 */
#define sys_port_trace_k_mbox_get_blocking(mbox, timeout)

/**
 * @brief Trace Mailbox get attempt outcome
 * @param mbox Mailbox entry
 * @param timeout Timeout period
 * @param ret Return value
 */
#define sys_port_trace_k_mbox_get_exit(mbox, timeout, ret)

/**
 * @brief Trace Mailbox data get
 * @brief rx_msg Receive Message object
 */
#define sys_port_trace_k_mbox_data_get(rx_msg)

/**
 * @}
 */ /* end of mbox_tracing_apis */




/**
 * @brief Pipe Tracing APIs
 * @defgroup pipe_tracing_apis Pipe Tracing APIs
 * @ingroup tracing_apis
 * @{
 */

/**
 * @brief Trace initialization of Pipe
 * @param pipe Pipe object
 */
#define sys_port_trace_k_pipe_init(pipe)

/**
 * @brief Trace Pipe cleanup entry
 * @param pipe Pipe object
 */
#define sys_port_trace_k_pipe_cleanup_enter(pipe)

/**
 * @brief Trace Pipe cleanup exit
 * @param pipe Pipe object
 * @param ret Return value
 */
#define sys_port_trace_k_pipe_cleanup_exit(pipe, ret)

/**
 * @brief Trace Pipe alloc init entry
 * @param pipe Pipe object
 */
#define sys_port_trace_k_pipe_alloc_init_enter(pipe)

/**
 * @brief Trace Pipe alloc init exit
 * @param pipe Pipe object
 * @param ret Return value
 */
#define sys_port_trace_k_pipe_alloc_init_exit(pipe, ret)

/**
 * @brief Trace Pipe put attempt entry
 * @param pipe Pipe object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_pipe_put_enter(pipe, timeout)

/**
 * @brief Trace Pipe put attempt blocking
 * @param pipe Pipe object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_pipe_put_blocking(pipe, timeout)

/**
 * @brief Trace Pipe put attempt outcome
 * @param pipe Pipe object
 * @param timeout Timeout period
 * @param ret Return value
 */
#define sys_port_trace_k_pipe_put_exit(pipe, timeout, ret)

/**
 * @brief Trace Pipe get attempt entry
 * @param pipe Pipe object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_pipe_get_enter(pipe, timeout)

/**
 * @brief Trace Pipe get attempt blocking
 * @param pipe Pipe object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_pipe_get_blocking(pipe, timeout)

/**
 * @brief Trace Pipe get attempt outcome
 * @param pipe Pipe object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_pipe_get_exit(pipe, timeout, ret)

/**
 * @brief Trace Pipe block put enter
 * @param pipe Pipe object
 * @param sem Semaphore object
 */
#define sys_port_trace_k_pipe_block_put_enter(pipe, sem)

/**
 * @brief Trace Pipe block put exit
 * @param pipe Pipe object
 * @param sem Semaphore object
 */
#define sys_port_trace_k_pipe_block_put_exit(pipe, sem)

/**
 * @}
 */ /* end of pipe_tracing_apis */




/**
 * @brief Heap Tracing APIs
 * @defgroup heap_tracing_apis Heap Tracing APIs
 * @ingroup tracing_apis
 * @{
 */

/**
 * @brief Trace initialization of Heap
 * @param h Heap object
 */
#define sys_port_trace_k_heap_init(h)

/**
 * @brief Trace Heap aligned alloc attempt entry
 * @param h Heap object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_heap_aligned_alloc_enter(h, timeout)

/**
 * @brief Trace Heap align alloc attempt blocking
 * @param h Heap object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_heap_aligned_alloc_blocking(h, timeout)

/**
 * @brief Trace Heap align alloc attempt outcome
 * @param h Heap object
 * @param timeout Timeout period
 * @param ret Return value
 */
#define sys_port_trace_k_heap_aligned_alloc_exit(h, timeout, ret)

/**
 * @brief Trace Heap alloc enter
 * @param h Heap object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_heap_alloc_enter(h, timeout)

/**
 * @brief Trace Heap alloc exit
 * @param h Heap object
 * @param timeout Timeout period
 * @param ret Return value
 */
#define sys_port_trace_k_heap_alloc_exit(h, timeout, ret)

/**
 * @brief Trace Heap free
 * @param h Heap object
 */
#define sys_port_trace_k_heap_free(h)

/**
 * @brief Trace System Heap aligned alloc enter
 * @param heap Heap object
 */
#define sys_port_trace_k_heap_sys_k_aligned_alloc_enter(heap)

/**
 * @brief Trace System Heap aligned alloc exit
 * @param heap Heap object
 * @param ret Return value
 */
#define sys_port_trace_k_heap_sys_k_aligned_alloc_exit(heap, ret)

/**
 * @brief Trace System Heap aligned alloc enter
 * @param heap Heap object
 */
#define sys_port_trace_k_heap_sys_k_malloc_enter(heap)

/**
 * @brief Trace System Heap aligned alloc exit
 * @param heap Heap object
 * @param ret Return value
 */
#define sys_port_trace_k_heap_sys_k_malloc_exit(heap, ret)

/**
 * @brief Trace System Heap free entry
 * @param heap Heap object
 */
#define sys_port_trace_k_heap_sys_k_free_enter(heap)

/**
 * @brief Trace System Heap free exit
 * @param heap Heap object
 */
#define sys_port_trace_k_heap_sys_k_free_exit(heap)

/**
 * @brief Trace System heap calloc enter
 * @param heap
 */
#define sys_port_trace_k_heap_sys_k_calloc_enter(heap)

/**
 * @brief Trace System heap calloc exit
 * @param heap Heap object
 * @param ret Return value
 */
#define sys_port_trace_k_heap_sys_k_calloc_exit(heap, ret)

/**
 * @}
 */ /* end of heap_tracing_apis */




/**
 * @brief Memory Slab Tracing APIs
 * @defgroup mslab_tracing_apis Memory Slab Tracing APIs
 * @ingroup tracing_apis
 * @{
 */

/**
 * @brief Trace initialization of Memory Slab
 * @param slab Memory Slab object
 * @param rc Return value
 */
#define sys_port_trace_k_mem_slab_init(slab, rc)

/**
 * @brief Trace Memory Slab alloc attempt entry
 * @param slab Memory Slab object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_mem_slab_alloc_enter(slab, timeout)

/**
 * @brief Trace Memory Slab alloc attempt blocking
 * @param slab Memory Slab object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_mem_slab_alloc_blocking(slab, timeout)

/**
 * @brief Trace Memory Slab alloc attempt outcome
 * @param slab Memory Slab object
 * @param timeout Timeout period
 * @param ret Return value
 */
#define sys_port_trace_k_mem_slab_alloc_exit(slab, timeout, ret)

/**
 * @brief Trace Memory Slab free entry
 * @param slab Memory Slab object
 */
#define sys_port_trace_k_mem_slab_free_enter(slab)

/**
 * @brief Trace Memory Slab free exit
 * @param slab Memory Slab object
 */
#define sys_port_trace_k_mem_slab_free_exit(slab)

/**
 * @}
 */ /* end of mslab_tracing_apis */




/**
 * @brief Timer Tracing APIs
 * @defgroup timer_tracing_apis Timer Tracing APIs
 * @ingroup tracing_apis
 * @{
 */

/**
 * @brief Trace initialization of Timer
 * @param timer Timer object
 */
#define sys_port_trace_k_timer_init(timer)

/**
 * @brief Trace Timer start
 * @param timer Timer object
 */
#define sys_port_trace_k_timer_start(timer)

/**
 * @brief Trace Timer stop
 * @param timer Timer object
 */
#define sys_port_trace_k_timer_stop(timer)

/**
 * @brief Trace Timer status sync entry
 * @param timer Timer object
 */
#define sys_port_trace_k_timer_status_sync_enter(timer)

/**
 * @brief Trace Timer Status sync blocking
 * @param timer Timer object
 * @param timeout Timeout period
 */
#define sys_port_trace_k_timer_status_sync_blocking(timer, timeout)

/**
 * @brief Trace Time Status sync outcome
 * @param timer Timer object
 * @param result Return value
 */
#define sys_port_trace_k_timer_status_sync_exit(timer, result)

/**
 * @}
 */ /* end of timer_tracing_apis */

/**
 * @}
 */

#endif
#endif
