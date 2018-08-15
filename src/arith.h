#ifndef ARITH_H
#define ARITH_H
#include <stdlib.h>
#include "core.h"
#include "namespace.h"
#define FUNC_FROM_MATHOP(op,name) \
obj_t *name(obj_t *x,obj_t *y) \
{ \
	bool type_err=false; \
	type_err|=!type_check(x,INTEGER|DOUBLE,#op", arg 1: "); \
	type_err|=!type_check(y,INTEGER|DOUBLE,#op", arg 2: "); \
	if (type_err) \
		return new_object(); \
	if (x->type==INTEGER&&y->type==INTEGER) \
		return new_integer(x->data.i op y->data.i); \
	double a=dub(x),b=dub(y); \
	return new_double(a op b); \
} \
STACK(name,2)
#define FUNC_FROM_COMP(op,name) \
obj_t *name(obj_t *x,obj_t *y) \
{ \
	bool type_err=false; \
	type_err|=!type_check(x,INTEGER|DOUBLE,#op", arg 1: "); \
	type_err|=!type_check(y,INTEGER|DOUBLE,#op", arg 2: "); \
	if (type_err) \
		return new_object(); \
	double a=dub(x),b=dub(y); \
	if (a op b) \
		return strsym("T"); \
	else \
		return NULL; \
} \
STACK(name,2)
#define FUNC_FROM_BOOLOP(op,name) \
obj_t *name(obj_t *x,obj_t *y) \
{ \
	if (x op y) \
		return strsym("T"); \
	else \
		return NULL; \
} \
STACK(name,2)

void init_arith(void);
#endif
