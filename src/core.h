// Functions for use in the LISP environment
// and macros to produce them from C functions
#ifndef CORE_H
#define CORE_H
#include "compile.h"
#include "function.h"
#include "hash.h"
#include "interpret.h"
#include "namespace.h"
#include "stack.h"
#include "object.h"
// Stack function declaration
#define STACK_FUNC(func) \
	void stack_##func(void)
#define CORE(name,func,...) \
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


CORE(PRINT,print,obj_t *)
CORE(TERPRI,terpri,void)
CORE(CONS,cons,obj_t *,obj_t *)
CORE(CAR,car,obj_t *)
CORE(CDR,cdr,obj_t *)
CORE(RPLACA,rplaca,obj_t *,obj_t *)
CORE(RPLACD,rplacd,obj_t *,obj_t *)
CORE(ATOM,atom,obj_t *)
CORE(SET,set,obj_t *,obj_t *)
CORE(GET,get,obj_t *)
CORE(UNSET,unset,obj_t *)
CORE(EQ,eq,obj_t *,obj_t *)
CORE(COPY,copy,obj_t *)
CORE(LENGTH,length,obj_t *)
CORE(LAMBDA,lambda,obj_t *,obj_t *)
CORE(EVAL,eval,obj_t *)
#endif
