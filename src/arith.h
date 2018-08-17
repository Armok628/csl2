#ifndef ARITH_H
#define ARITH_H
#include <math.h>
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
#define FUNC_FROM_BITOP(op,name) \
obj_t *name(obj_t *x,obj_t *y) \
{ \
	bool type_err=false; \
	type_err|=!type_check(x,INTEGER,#op", arg 1: "); \
	type_err|=!type_check(y,INTEGER,#op", arg 2: "); \
	if (type_err) \
		return new_object(); \
	return new_integer(x->data.i op y->data.i); \
} \
STACK(name,2)
#define FUNC_FROM_MATHFUNC_1(func) \
obj_t *l_##func(obj_t *x) \
{ \
	double d=dub(x); \
	d=func(d); \
	if (fmod(d,1.0)<1e-6) \
		return new_integer((long)d); \
	return new_double(d); \
} \
STACK(l_##func,1)
#define FUNC_FROM_MATHFUNC_2(func) \
obj_t *l_##func(obj_t *x,obj_t *y) \
{ \
	double a=dub(x),b=dub(y); \
	double d=func(a,b); \
	if (fmod(d,1.0)<1e-6) \
		return new_integer((long)d); \
	return new_double(d); \
} \
STACK(l_##func,2)
#define FUNCS_FROM_TRIG(func) \
FUNC_FROM_MATHFUNC_1(func) \
FUNC_FROM_MATHFUNC_1(func##h) \
FUNC_FROM_MATHFUNC_1(a##func) \
FUNC_FROM_MATHFUNC_1(a##func##h)

void init_arith(void);
#endif
