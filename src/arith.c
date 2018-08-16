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
obj_t *lrandom(obj_t *n)
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
STACK(lrandom,1)
void init_arith(void)
{
	srand(time(NULL));
	INIT(*,multiply);
	INIT(+,add);
	INIT(-,subtract);
	INIT(/,divide);
	INIT(<,lt);
	INIT(<=,lte);
	INIT(==,eqn);
	INIT(>,gt);
	INIT(>=,gte);
	INIT(AND,and);
	INIT(OR,or);
	INIT(RANDOM,lrandom);
}
