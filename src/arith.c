#include "arith.h"
static inline double dub(obj_t *n)
{
	return n->type==INTEGER?(double)n->data.i:n->data.d;
}
obj_t *l_random(obj_t *n)
{
	if (!type_check(n,DOUBLE|INTEGER,"RANDOM: "))
		return error;
	switch (n->type) {
	case INTEGER:
		return new_integer(rand()%n->data.i);
	case DOUBLE:
		return new_double((rand()/(double)RAND_MAX)*n->data.d);
	default:
		return NULL;
	}
}
STACK(l_random,1)
#define FUNC_FROM_MATHOP(op,name) \
obj_t *name(obj_t *x,obj_t *y) \
{ \
	bool type_err=false; \
	type_err|=!type_check(x,INTEGER|DOUBLE,#op", arg 1: "); \
	type_err|=!type_check(y,INTEGER|DOUBLE,#op", arg 2: "); \
	if (type_err) \
		return error; \
	if (x->type==INTEGER&&y->type==INTEGER) \
		return new_integer(x->data.i op y->data.i); \
	double a=dub(x),b=dub(y); \
	return new_double(a op b); \
} \
STACK(name,2)
FUNC_FROM_MATHOP(+,add)
FUNC_FROM_MATHOP(-,subtract)
FUNC_FROM_MATHOP(*,multiply)
FUNC_FROM_MATHOP(/,divide)
#define FUNC_FROM_COMP(op,name) \
obj_t *name(obj_t *x,obj_t *y) \
{ \
	bool type_err=false; \
	type_err|=!type_check(x,INTEGER|DOUBLE,#op", arg 1: "); \
	type_err|=!type_check(y,INTEGER|DOUBLE,#op", arg 2: "); \
	if (type_err) \
		return error; \
	double a=dub(x),b=dub(y); \
	if (a op b) \
		return T; \
	else \
		return NULL; \
} \
STACK(name,2)
FUNC_FROM_COMP(>,gt)
FUNC_FROM_COMP(>=,gte)
FUNC_FROM_COMP(<,lt)
FUNC_FROM_COMP(<=,lte)
FUNC_FROM_COMP(==,eqn)
#define FUNC_FROM_BOOLOP(op,name) \
obj_t *name(obj_t *x,obj_t *y) \
{ \
	if (x op y) \
		return T; \
	else \
		return NULL; \
} \
STACK(name,2)
FUNC_FROM_BOOLOP(||,or)
FUNC_FROM_BOOLOP(&&,and)
#define FUNC_FROM_MATHFUNC_2(func) \
obj_t *l_##func(obj_t *x,obj_t *y) \
{ \
	bool type_err=false; \
	type_err|=!type_check(x,INTEGER|DOUBLE,#func"(), arg 1: "); \
	type_err|=!type_check(y,INTEGER|DOUBLE,#func"(), arg 2: "); \
	if (type_err) \
		return error; \
	double a=dub(x),b=dub(y); \
	double d=func(a,b); \
	if (fmod(d,1.0)<1e-6) \
		return new_integer((long)d); \
	return new_double(d); \
} \
STACK(l_##func,2)
FUNC_FROM_MATHFUNC_2(pow)
FUNC_FROM_MATHFUNC_2(fmod)
#define FUNC_FROM_MATHFUNC_1(func) \
obj_t *l_##func(obj_t *x) \
{ \
	if (!type_check(x,INTEGER|DOUBLE,#func"(): ")) \
		return error; \
	double d=dub(x); \
	d=func(d); \
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
void init_arith(void)
{
	srand(time(NULL));
	insert(dict,"PI",incr_refs(&pi_obj));
	INIT(*,multiply)
	INIT(+,add)
	INIT(-,subtract)
	INIT(/,divide)
	INIT(<,lt)
	INIT(<=,lte)
	INIT(==,eqn)
	INIT(>,gt)
	INIT(>=,gte)
	INIT(AND,and)
	INIT(OR,or)
	INIT(RANDOM,l_random)
	INIT(EXPT,l_pow)
	INIT(MOD,l_fmod)
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
