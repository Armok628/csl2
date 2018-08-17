#include "arith.h"
static inline double dub(obj_t *n)
{
	return n->type==INTEGER?(double)n->data.i:n->data.d;
}
FUNC_FROM_MATHOP(+,add)
FUNC_FROM_MATHOP(-,subtract)
FUNC_FROM_MATHOP(*,multiply)
FUNC_FROM_MATHOP(/,divide)
FUNC_FROM_COMP(>,gt)
FUNC_FROM_COMP(>=,gte)
FUNC_FROM_COMP(<,lt)
FUNC_FROM_COMP(>=,lte)
FUNC_FROM_COMP(==,eqn)
FUNC_FROM_BOOLOP(||,or)
FUNC_FROM_BOOLOP(&&,and)
obj_t *l_random(obj_t *n)
{
	if (!type_check(n,DOUBLE|INTEGER,"RANDOM: "))
		return new_object();
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
FUNC_FROM_MATHFUNC_2(pow)
FUNC_FROM_MATHFUNC_2(fmod)
FUNC_FROM_MATHFUNC_1(floor)
FUNC_FROM_MATHFUNC_1(ceil)
FUNC_FROM_MATHFUNC_1(round)
FUNC_FROM_MATHFUNC_1(trunc)
FUNC_FROM_MATHFUNC_1(sqrt)
FUNC_FROM_MATHFUNC_1(exp)
FUNC_FROM_MATHFUNC_1(log)
FUNCS_FROM_TRIG(sin)
FUNCS_FROM_TRIG(cos)
FUNCS_FROM_TRIG(tan)
void init_arith(void)
{
	srand(time(NULL));
	insert(dict,"PI",incr_refs(new_double(M_PI)));
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
