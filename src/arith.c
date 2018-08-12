#include "arith.h"
void init_arith(void)
{
	INIT(+,add);
}
static inline double dub(obj_t *n)
{
	return n->type==INTEGER?(double)n->data.i:n->data.d;
}
STACK(add,2_ARGS)
obj_t *add(obj_t *x,obj_t *y)
{
	if (!type_check(x,INTEGER|DOUBLE,"+, arg 1: "))
		return new_object();
	if (!type_check(y,INTEGER|DOUBLE,"+, arg 2: "))
		return new_object();
	if (x->type==INTEGER&&y->type==INTEGER)
		return new_integer(x->data.i+y->data.i);
	double a=dub(x),b=dub(y);
	return new_double(a+b);
}
