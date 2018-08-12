#ifndef ARITH_H
#define ARITH_H
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

void init_arith(void);
CORE(add,obj_t *,obj_t *)
CORE(subtract,obj_t *,obj_t *)
CORE(multiply,obj_t *,obj_t *)
CORE(divide,obj_t *,obj_t *)
#endif
