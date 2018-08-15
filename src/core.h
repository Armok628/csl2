// Functions for use in the LISP environment
// and macros to produce them from C functions
#ifndef CORE_H
#define CORE_H
#include "compile.h"
#include "function.h"
#include "hash.h"
#include "interpret.h"
#include "namespace.h"
#include "object.h"
#include "parser.h"
#include "stack.h"
#include "timer.h"
// Stack function declaration
#define STACK_FUNC(func) \
	void stack_##func(void)
#define CORE(func,...) \
	obj_t *func(__VA_ARGS__); \
	STACK_FUNC(func);
// C functions to stack functions
#define GETARGS(n) GETARG##n
#define GETARG4 obj_t *arg4=pop(); GETARG3
#define GETARG3 obj_t *arg3=pop(); GETARG2
#define GETARG2 obj_t *arg2=pop(); GETARG1
#define GETARG1 obj_t *arg1=pop(); GETARG0
#define GETARG0
#define ARGS(n) ARG##n
#define ARG4 ARG3,arg4
#define ARG3 ARG2,arg3
#define ARG2 ARG1,arg2
#define ARG1 arg1
#define ARG0
#define DECRREFS(n) DECRREF##n
#define DECRREF4 decr_refs(arg4); DECRREF3
#define DECRREF3 decr_refs(arg3); DECRREF2
#define DECRREF2 decr_refs(arg2); DECRREF1
#define DECRREF1 decr_refs(arg1); DECRREF0
#define DECRREF0

#define STACK(func,n) \
	STACK_FUNC(func) \
	{ \
		GETARGS(n) \
		push(func(ARGS(n))); \
		DECRREFS(n) \
	}
#define INIT(name,func) \
	insert(dict,#name,incr_refs(new_cfunction(&stack_##func)));
// Files depending on above macros are part of the core:
#include "arith.h"

void init_core(void);
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
CORE(lambda,obj_t *,obj_t *)
CORE(eval,obj_t *)
CORE(lread,obj_t *)
CORE(null,obj_t *)
CORE(quit,void)
CORE(see,obj_t *)
CORE(tick,void)
CORE(tock,void)
CORE(uplevel,obj_t *,obj_t *)
CORE(nconc,obj_t *,obj_t *)
CORE(lfor,obj_t *,obj_t *,obj_t *,obj_t *)
#endif
