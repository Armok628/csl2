#include "lmath.h"
static inline double obj2double(obj_t *n)
{ // Taken from arith.c
	return n->type==INTEGER?(double)n->data.i:n->data.d;
}
#define FUNC_FROM_MATHFUNC_2(func) \
obj_t *l_##func(obj_t *x,obj_t *y) \
{ \
	bool type_err=false; \
	type_err|=!type_check(x,INTEGER|DOUBLE,#func"(), arg 1: "); \
	type_err|=!type_check(y,INTEGER|DOUBLE,#func"(), arg 2: "); \
	if (type_err) \
		return error; \
	double d=func(obj2double(x),obj2double(y)); \
	if (fmod(d,1.0)<1e-6) \
		return new_integer((long)d); \
	return new_double(d); \
} \
STACK(l_##func,2)
FUNC_FROM_MATHFUNC_2(pow)
#define FUNC_FROM_MATHFUNC_1(func) \
obj_t *l_##func(obj_t *x) \
{ \
	if (!type_check(x,INTEGER|DOUBLE,#func"(): ")) \
		return error; \
	double d=func(obj2double(x)); \
	if (fmod(d,1.0)<1e-6) \
		return new_integer((long)d); \
	return new_double(d); \
} \
STACK(l_##func,1)
FUNC_FROM_MATHFUNC_1(floor)
FUNC_FROM_MATHFUNC_1(ceil)
FUNC_FROM_MATHFUNC_1(round)
FUNC_FROM_MATHFUNC_1(trunc)
FUNC_FROM_MATHFUNC_1(sqrt)
FUNC_FROM_MATHFUNC_1(exp)
FUNC_FROM_MATHFUNC_1(log)
#define FUNCS_FROM_TRIG(func) \
FUNC_FROM_MATHFUNC_1(func) \
FUNC_FROM_MATHFUNC_1(func##h) \
FUNC_FROM_MATHFUNC_1(a##func) \
FUNC_FROM_MATHFUNC_1(a##func##h)
FUNCS_FROM_TRIG(sin)
FUNCS_FROM_TRIG(cos)
FUNCS_FROM_TRIG(tan)
obj_t pi_obj={
	.type=DOUBLE,
	.data={.d=M_PI},
	.refs=1,
};
void init_math(void)
{
	insert(dict,"PI",incr_refs(&pi_obj));
	INIT(EXPT,l_pow)
	INIT(FLOOR,l_floor)
	INIT(CEIL,l_ceil)
	INIT(ROUND,l_round)
	INIT(TRUNCATE,l_trunc)
	INIT(SQRT,l_sqrt)
	INIT(EXP,l_exp)
	INIT(LOG,l_log)
	INIT(SIN,l_sin)
	INIT(ASIN,l_asin)
	INIT(SINH,l_sinh)
	INIT(ASINH,l_asinh)
	INIT(COS,l_cos)
	INIT(ACOS,l_acos)
	INIT(COSH,l_cosh)
	INIT(ACOSH,l_acosh)
	INIT(TAN,l_tan)
	INIT(ATAN,l_atan)
	INIT(TANH,l_tanh)
	INIT(ATANH,l_atanh)
}
