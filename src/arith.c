#include "arith.h"
void init_arith(void)
{
	srand(time(NULL));
	INIT(+,add);
	INIT(-,subtract);
	INIT(*,multiply);
	INIT(/,divide);
	INIT(>,gt);
	INIT(>=,gte);
	INIT(<,lt);
	INIT(<=,lte);
	INIT(==,eqn);
	INIT(RANDOM,lrandom);
}
static inline double dub(obj_t *n)
{
	return n->type==INTEGER?(double)n->data.i:n->data.d;
}
FUNC_FROM_OP(+,add)
FUNC_FROM_OP(-,subtract)
FUNC_FROM_OP(*,multiply)
FUNC_FROM_OP(/,divide)
FUNC_FROM_COMP(>,gt)
FUNC_FROM_COMP(>=,gte)
FUNC_FROM_COMP(<,lt)
FUNC_FROM_COMP(>=,lte)
FUNC_FROM_COMP(==,eqn)
STACK(lrandom,1_ARG)
obj_t *lrandom(obj_t *n)
{
	if (!type_check(n,DOUBLE|INTEGER,"RANDOM: "))
		return NULL;
	switch (n->type) {
	case INTEGER:
		return new_integer(rand()%n->data.i);
	case DOUBLE:
		return new_double((rand()/(double)RAND_MAX)*n->data.d);
	default:
		return NULL;
	}
}
