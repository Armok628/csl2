// Functions for use in the LISP environment
// and macros to produce them from C functions
#ifndef CORE_H
#define CORE_H
#include <time.h>
#include "compile.h"
#include "consts.h"
#include "function.h"
#include "hash.h"
#include "interpret.h"
#include "namespace.h"
#include "object.h"
#include "parser.h"
#include "stack.h"
// Warning: Preprocessor abuse ahead
#define GETARGS(n) GETARG##n
#define GETARG4 obj_t *arg4=pop(); GETARG3
#define GETARG3 obj_t *arg3=pop(); GETARG2
#define GETARG2 obj_t *arg2=pop(); GETARG1
#define GETARG1 obj_t *arg1=pop();
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
#define DECRREF1 decr_refs(arg1);
#define DECRREF0
// C functions to stack functions
// Use these macros to generate functions available to the language
#define STACK(function,n) \
void stack_##function(void) \
{ \
	GETARGS(n) \
	push(function(ARGS(n))); \
	DECRREFS(n) \
} \
obj_t stack_##function##_func={ \
	.type=FUNCTION, \
	.data={.func={.lambda=false,.rep={.c=&stack_##function}}}, \
	.refs=1, \
};
// ^ This one goes after your function definition.
#define INIT(name,function) \
insert(dict,#name,incr_refs(&stack_##function##_func));
// ^ This one goes inside your init function.

void init_core(void);
#endif
