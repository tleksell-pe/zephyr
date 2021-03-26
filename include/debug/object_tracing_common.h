/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief APIs used to add or remove an object in a debug tracing list.
 */

#ifndef ZEPHYR_INCLUDE_DEBUG_OBJECT_TRACING_COMMON_H_
#define ZEPHYR_INCLUDE_DEBUG_OBJECT_TRACING_COMMON_H_

#include <stdbool.h>

#ifndef CONFIG_OBJECT_TRACING

#define SYS_TRACING_OBJ_INIT(name, obj) do { } while (false)
#define SYS_TRACING_OBJ_INIT_DLL(name, obj) do { } while (false)
#define SYS_TRACING_OBJ_REMOVE_DLL(name, obj) do { } while (false)

/**
 * @note	Extended tracing macro blanks
 */
#define SYS_PORT_TRACING_FUNC(type, func, ...) do { } while (false)
#define SYS_PORT_TRACING_FUNC_ENTER(type, func, ...) do { } while (false)
#define SYS_PORT_TRACING_FUNC_BLOCKING(type, func, ...) do { } while (false)
#define SYS_PORT_TRACING_FUNC_EXIT(type, func, ...) do { } while (false)
#define SYS_PORT_TRACING_OBJ_INIT(obj_type, obj, ...) do { } while (false)
#define SYS_PORT_TRACING_OBJ_FUNC(obj_type, func, obj, ...) do { } while (false)
#define SYS_PORT_TRACING_OBJ_FUNC_ENTER(obj_type, func, obj, ...) do { } while (false)
#define SYS_PORT_TRACING_OBJ_FUNC_BLOCKING(obj_type, func, obj, ...) do { } while (false)
#define SYS_PORT_TRACING_OBJ_FUNC_EXIT(obj_type, func, obj, ...) do { } while (false)


#else

/**
 * @def SYS_TRACING_OBJ_INIT
 *
 * @brief Adds a new object into the trace list
 *
 * @details The object is added for tracing into a trace list. This is usually
 * called at the moment of object initialization.
 *
 * @param name Name of the trace list.
 * @param obj Object to be added in the trace list.
 */
#define SYS_TRACING_OBJ_INIT(name, obj)			       \
	do {						       \
		unsigned int key;			       \
							       \
		key = irq_lock();			       \
		if (!(obj)->__linked) {			       \
			(obj)->__next =  _trace_list_ ## name; \
			_trace_list_ ## name = obj;	       \
			(obj)->__linked = 1;		       \
		}					       \
		irq_unlock(key);			       \
	}						       \
	while (false)

/**
 * @def SYS_TRACING_OBJ_INIT_DLL
 *
 * @brief Adds a new object into the trace list as a double linked list.
 *
 * @details The object is added for tracing into a trace list. This is usually
 * called at the moment of object initialization. This list is used for objects
 * that can be removed from the tracing list dynamically.
 *
 * @param name Name of the trace list.
 * @param obj Object to be added in the trace list.
 */
#define SYS_TRACING_OBJ_INIT_DLL(name, obj)		      \
	do {						      \
		unsigned int key;			      \
							      \
		key = irq_lock();			      \
		if (_trace_list_ ## name) {		      \
			_trace_list_ ## name->__prev = (obj); \
		}					      \
		(obj)->__next = _trace_list_ ## name;	      \
		(obj)->__prev = NULL;			      \
		_trace_list_ ## name = obj;		      \
		irq_unlock(key);			      \
	}						      \
	while (false)

/**
 * @def SYS_TRACING_OBJ_REMOVE_DLL
 *
 * @brief Removes an object from a double linked trace list.
 *
 * @details The object is remove from the trace list.
 * It needs to be used with DEBUG_TRACING_OBJ_INIT_DLL as a pair.
 *
 * @param name Name of the trace list.
 * @param obj Object to be removed from the trace list.
 */
#define SYS_TRACING_OBJ_REMOVE_DLL(name, obj)		      \
	do {						      \
		unsigned int key;			      \
							      \
		key = irq_lock();			      \
		if (obj->__next) {			      \
			obj->__next->__prev = (obj)->__prev;  \
		}					      \
		if (obj->__prev) {			      \
			obj->__prev->__next = (obj)->__next;  \
		} else {				      \
			_trace_list_ ## name = (obj)->__next; \
		}					      \
		irq_unlock(key);			      \
	}						      \
	while (false)


/* Helper macros used by the extended tracing system
 */
#define _SYS_PORT_TRACING_TYPE_MASK(type) \
	sys_port_trace_type_mask_ ## type
#define _SYS_PORT_TRACING_FUNC(name, func) \
	sys_port_trace_ ## name ## _ ## func
#define _SYS_PORT_TRACING_FUNC_ENTER(name, func) \
	sys_port_trace_ ## name ## _ ## func ## _enter
#define _SYS_PORT_TRACING_FUNC_BLOCKING(name, func) \
	sys_port_trace_ ## name ## _ ## func ## _blocking
#define _SYS_PORT_TRACING_FUNC_EXIT(name, func) \
	sys_port_trace_ ## name ## _ ## func ## _exit
#define _SYS_PORT_TRACING_OBJ_INIT(name) \
	sys_port_trace_ ## name ## _init
#define _SYS_PORT_TRACING_OBJ_FUNC(name, func) \
	sys_port_trace_ ## name ## _ ## func
#define _SYS_PORT_TRACING_OBJ_FUNC_ENTER(name, func) \
	sys_port_trace_ ## name ## _ ## func ## _enter
#define _SYS_PORT_TRACING_OBJ_FUNC_BLOCKING(name, func) \
	sys_port_trace_ ## name ## _ ## func ## _blocking
#define _SYS_PORT_TRACING_OBJ_FUNC_EXIT(name, func) \
	sys_port_trace_ ## name ## _ ## func ## _exit


/* Object trace macros part of the system for checking if certain
 * objects should be traced or not depending on the tracing configuration.
 */
#if defined(CONFIG_TRACING_THREAD)
	#define sys_port_trace_type_mask_k_thread(trace_call) trace_call
#else
	#define sys_port_trace_type_mask_k_thread(trace_call)
#endif

#if defined(CONFIG_TRACING_WORK)
	#define sys_port_trace_type_mask_k_work(trace_call) trace_call
	#define sys_port_trace_type_mask_k_work_queue(trace_call) trace_call
	#define sys_port_trace_type_mask_k_work_delayable(trace_call) trace_call
	#define sys_port_trace_type_mask_k_work_poll(trace_call) trace_call
#else
	#define sys_port_trace_type_mask_k_work(trace_call)
	#define sys_port_trace_type_mask_k_work_queue(trace_call)
	#define sys_port_trace_type_mask_k_work_delayable(trace_call)
	#define sys_port_trace_type_mask_k_work_poll(trace_call)
#endif

#if defined(CONFIG_TRACING_SEMAPHORE)
	#define sys_port_trace_type_mask_k_sem(trace_call) trace_call
#else
	#define sys_port_trace_type_mask_k_sem(trace_call)
#endif

#if defined(CONFIG_TRACING_MUTEX)
	#define sys_port_trace_type_mask_k_mutex(trace_call) trace_call
#else
	#define sys_port_trace_type_mask_k_mutex(trace_call)
#endif

#if defined(CONFIG_TRACING_CONDVAR)
	#define sys_port_trace_type_mask_k_condvar(trace_call) trace_call
#else
	#define sys_port_trace_type_mask_k_condvar(trace_call)
#endif

#if defined(CONFIG_TRACING_QUEUE)
	#define sys_port_trace_type_mask_k_queue(trace_call) trace_call
#else
	#define sys_port_trace_type_mask_k_queue(trace_call)
#endif

#if defined(CONFIG_TRACING_FIFO)
	#define sys_port_trace_type_mask_k_fifo(trace_call) trace_call
#else
	#define sys_port_trace_type_mask_k_fifo(trace_call)
#endif

#if defined(CONFIG_TRACING_LIFO)
	#define sys_port_trace_type_mask_k_lifo(trace_call) trace_call
#else
	#define sys_port_trace_type_mask_k_lifo(trace_call)
#endif

#if defined(CONFIG_TRACING_STACK)
	#define sys_port_trace_type_mask_k_stack(trace_call) trace_call
#else
	#define sys_port_trace_type_mask_k_stack(trace_call)
#endif

#if defined(CONFIG_TRACING_MESSAGE_QUEUE)
	#define sys_port_trace_type_mask_k_msgq(trace_call) trace_call
#else
	#define sys_port_trace_type_mask_k_msgq(trace_call)
#endif

#if defined(CONFIG_TRACING_MAILBOX)
	#define sys_port_trace_type_mask_k_mbox(trace_call) trace_call
#else
	#define sys_port_trace_type_mask_k_mbox(trace_call)
#endif

#if defined(CONFIG_TRACING_PIPE)
	#define sys_port_trace_type_mask_k_pipe(trace_call) trace_call
#else
	#define sys_port_trace_type_mask_k_pipe(trace_call)
#endif

#if defined(CONFIG_TRACING_HEAP)
	#define sys_port_trace_type_mask_k_heap(trace_call) trace_call
	#define sys_port_trace_type_mask_k_heap_sys(trace_call) trace_call
#else
	#define sys_port_trace_type_mask_k_heap(trace_call)
	#define sys_port_trace_type_mask_k_heap_sys(trace_call)
#endif

#if defined(CONFIG_TRACING_MEMORY_SLAB)
	#define sys_port_trace_type_mask_k_mem_slab(trace_call) trace_call
#else
	#define sys_port_trace_type_mask_k_mem_slab(trace_call)
#endif

#if defined(CONFIG_TRACING_TIMER)
	#define sys_port_trace_type_mask_k_timer(trace_call) trace_call
#else
	#define sys_port_trace_type_mask_k_timer(trace_call)
#endif




/**
 * @def SYS_PORT_TRACING_OBJ_MASK
 *
 * @brief Checks if an object type should be traced or not.
 *
 * @param type Tracing event type/object
 * @param trace_call Tracing call
 */
#define SYS_PORT_TRACING_TYPE_MASK(type, trace_call) \
	_SYS_PORT_TRACING_TYPE_MASK(type)(trace_call)




/**
 * @def SYS_PORT_TRACING_FUNC
 *
 * @brief Tracing macro for function calls which are not directly
 * associated with a specific type of object.
 *
 * @param type Type of tracing event or object type
 * @param func Name of the function responsible for the call. This does not need to exactly
 * match the name of the function but should rather match what the user called in case of
 * system calls etc. That is, we can often omit the z_vrfy/z_impl part of the name.
 * @param ... Additional parameters relevant to the tracing call
 */
#define SYS_PORT_TRACING_FUNC(type, func, ...) \
	do { \
		_SYS_PORT_TRACING_FUNC(type, func)(__VA_ARGS__); \
	} while (false)

/**
 * @def SYS_PORT_TRACING_FUNC_ENTER
 *
 * @brief Tracing macro for the entry into a function that might or might not return
 * a value.
 *
 * @param type Type of tracing event or object type
 * @param func Name of the function responsible for the call. This does not need to exactly
 * match the name of the function but should rather match what the user called in case of
 * system calls etc. That is, we can often omit the z_vrfy/z_impl part of the name.
 * @param ... Additional parameters relevant to the tracing call
 */
#define SYS_PORT_TRACING_FUNC_ENTER(type, func, ...) \
	do { \
		_SYS_PORT_TRACING_FUNC_ENTER(type, func)(__VA_ARGS__); \
	} while (false)

/**
 * @def SYS_PORT_TRACING_FUNC_BLOCKING
 *
 * @brief Tracing macro for when a function blocks during its execution.
 *
 * @param type Type of tracing event or object type
 * @param func Name of the function responsible for the call. This does not need to exactly
 * match the name of the function but should rather match what the user called in case of
 * system calls etc. That is, we can often omit the z_vrfy/z_impl part of the name.
 * @param ... Additional parameters relevant to the tracing call
 */
#define SYS_PORT_TRACING_FUNC_BLOCKING(type, func, ...) \
	do { \
		_SYS_PORT_TRACING_FUNC_BLOCKING(type, func)(__VA_ARGS__); \
	} while (false)

/**
 * @def SYS_PORT_TRACING_FUNC_EXIT
 *
 * @brief Tracing macro for when a function ends its execution. Potential return values
 * can be given as additional arguments.
 *
 * @param type Type of tracing event or object type
 * @param func Name of the function responsible for the call. This does not need to exactly
 * match the name of the function but should rather match what the user called in case of
 * system calls etc. That is, we can often omit the z_vrfy/z_impl part of the name.
 * @param ... Additional parameters relevant to the tracing call
 */
#define SYS_PORT_TRACING_FUNC_EXIT(type, func, ...) \
	do { \
		_SYS_PORT_TRACING_FUNC_EXIT(type, func)(__VA_ARGS__); \
	} while (false)

/**
 * @def SYS_PORT_TRACING_OBJ_INIT
 *
 * @brief Tracing macro for the initialization of an object.
 *
 * @param obj_type The type of object associated with the call (k_thread, k_sem, k_mutex etc.)
 * @param obj Object
 */
#define SYS_PORT_TRACING_OBJ_INIT(obj_type, obj, ...) \
	do { \
		SYS_PORT_TRACING_TYPE_MASK(obj_type, \
			_SYS_PORT_TRACING_OBJ_INIT(obj_type)(obj, ##__VA_ARGS__)); \
	} while (false)

/**
 * @def SYS_PORT_TRACING_OBJ_FUNC
 *
 * @brief Tracing macro for simple object function calls often without returns or branching.
 *
 * @param obj_type The type of object associated with the call (k_thread, k_sem, k_mutex etc.)
 * @param func Name of the function responsible for the call. This does not need to exactly
 * match the name of the function but should rather match what the user called in case of
 * system calls etc. That is, we can often omit the z_vrfy/z_impl part of the name.
 * @param obj Object
 * @param ... Additional parameters relevant to the tracing call
 */
#define SYS_PORT_TRACING_OBJ_FUNC(obj_type, func, obj, ...) \
	do { \
		SYS_PORT_TRACING_TYPE_MASK(obj_type, \
			_SYS_PORT_TRACING_OBJ_FUNC(obj_type, func)(obj, ##__VA_ARGS__)); \
	} while (false)

/**
 * @def SYS_PORT_TRACING_OBJ_FUNC_ENTER
 *
 * @brief Tracing macro for the entry into a function that might or might not return
 * a value.
 *
 * @param obj_type The type of object associated with the call (k_thread, k_sem, k_mutex etc.)
 * @param func Name of the function responsible for the call. This does not need to exactly
 * match the name of the function but should rather match what the user called in case of
 * system calls etc. That is, we can often omit the z_vrfy/z_impl part of the name.
 * @param obj Object
 * @param ... Additional parameters relevant to the tracing call
 */
#define SYS_PORT_TRACING_OBJ_FUNC_ENTER(obj_type, func, obj, ...) \
	do { \
		SYS_PORT_TRACING_TYPE_MASK(obj_type, \
			_SYS_PORT_TRACING_OBJ_FUNC_ENTER(obj_type, func)(obj, ##__VA_ARGS__)); \
	} while (false)

/**
 * @def SYS_PORT_TRACING_OBJ_FUNC_BLOCKING
 *
 * @brief Tracing macro for when a function blocks during its execution.
 *
 * @param obj_type The type of object associated with the call (k_thread, k_sem, k_mutex etc.)
 * @param func Name of the function responsible for the call. This does not need to exactly
 * match the name of the function but should rather match what the user called in case of
 * system calls etc. That is, we can often omit the z_vrfy/z_impl part of the name.
 * @param obj Object
 * @param ... Additional parameters relevant to the tracing call
 */
#define SYS_PORT_TRACING_OBJ_FUNC_BLOCKING(obj_type, func, obj, timeout, ...) \
	do { \
		SYS_PORT_TRACING_TYPE_MASK(obj_type, \
			_SYS_PORT_TRACING_OBJ_FUNC_BLOCKING(obj_type, func) \
			(obj, timeout, ##__VA_ARGS__)); \
	} while (false)

/**
 * @def SYS_PORT_TRACING_OBJ_FUNC_EXIT
 *
 * @brief Tracing macro for when a function ends its execution. Potential return values
 * can be given as additional arguments.
 *
 * @param obj_type The type of object associated with the call (k_thread, k_sem, k_mutex etc.)
 * @param func Name of the function responsible for the call. This does not need to exactly
 * match the name of the function but should rather match what the user called in case of
 * system calls etc. That is, we can often omit the z_vrfy/z_impl part of the name.
 * @param obj Object
 * @param ... Additional parameters relevant to the tracing call
 */
#define SYS_PORT_TRACING_OBJ_FUNC_EXIT(obj_type, func, obj, ...) \
	do { \
		SYS_PORT_TRACING_TYPE_MASK(obj_type, \
			_SYS_PORT_TRACING_OBJ_FUNC_EXIT(obj_type, func)(obj, ##__VA_ARGS__)); \
	} while (false)

#endif  /*CONFIG_OBJECT_TRACING*/
#endif  /*ZEPHYR_INCLUDE_DEBUG_OBJECT_TRACING_COMMON_H_*/
