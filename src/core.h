#ifndef CORE_H
#define CORE_H
#include "stack.h"
#include "types.h"
#include "hash.h"
// Stack function declaration
#define STACK_FUNC(func) \
	void stack_##func(void)
#define CORE(func,...) \
	obj_t *func(__VA_ARGS__); \
	STACK_FUNC(func);
// C functions to stack functions
#define STACK(func,n_args) \
	STACK_##n_args(func)
#define STACK_0_ARGS(func) \
	STACK_FUNC(func) \
	{ \
		push(func()); \
	}
#define STACK_1_ARG(func) \
	STACK_FUNC(func) \
	{ \
		obj_t *arg1=pop(); \
		push(func(arg1)); \
		decr_refs(arg1); \
	}
#define STACK_2_ARGS(func) \
	STACK_FUNC(func) \
	{ \
		obj_t *arg2=pop(); \
		obj_t *arg1=pop(); \
		push(func(arg1,arg2)); \
		decr_refs(arg1); \
		decr_refs(arg2); \
	}

extern table_t *dict;

CORE(print,obj_t *)
CORE(terpri,void)
CORE(cons,obj_t *,obj_t *)
CORE(car,obj_t *)
CORE(cdr,obj_t *)
CORE(rplaca,obj_t *,obj_t *)
CORE(rplacd,obj_t *,obj_t *)
CORE(atom,obj_t *)
CORE(set,obj_t *,obj_t *)
CORE(get,obj_t *)
CORE(unset,obj_t *)
CORE(eq,obj_t *,obj_t *)
CORE(copy,obj_t *)
CORE(length,obj_t *)
#endif
