#ifndef ARITH_H
#define ARITH_H
#include <stdlib.h>
#include "core.h"
#include "namespace.h"
#define FUNC_FROM_OP(op,name) \
STACK(name,2_ARGS) \
obj_t *name(obj_t *x,obj_t *y) \
{ \
	if (!type_check(x,INTEGER|DOUBLE,#op", arg 1: ")) \
		return new_object(); \
	if (!type_check(y,INTEGER|DOUBLE,#op", arg 2: ")) \
		return new_object(); \
	if (x->type==INTEGER&&y->type==INTEGER) \
		return new_integer(x->data.i op y->data.i); \
	double a=dub(x),b=dub(y); \
	return new_double(a op b); \
}
#define FUNC_FROM_COMP(op,name) \
STACK(name,2_ARGS) \
obj_t *name(obj_t *x,obj_t *y) \
{ \
	if (!type_check(x,INTEGER|DOUBLE,#op", arg 1: ")) \
		return new_object(); \
	if (!type_check(y,INTEGER|DOUBLE,#op", arg 2: ")) \
		return new_object(); \
	double a=dub(x),b=dub(y); \
	if (a op b) \
		return new_symbol(strdup("T")); \
	else \
		return NULL; \
}

void init_arith(void);
CORE(add,obj_t *,obj_t *)
CORE(subtract,obj_t *,obj_t *)
CORE(multiply,obj_t *,obj_t *)
CORE(divide,obj_t *,obj_t *)
CORE(gt,obj_t *,obj_t *)
CORE(gte,obj_t *,obj_t *)
CORE(lt,obj_t *,obj_t *)
CORE(lte,obj_t *,obj_t *)
CORE(eqn,obj_t *,obj_t *)
CORE(lrandom,obj_t *)
#endif
