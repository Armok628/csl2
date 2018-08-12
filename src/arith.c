#include "arith.h"
void init_arith(void)
{
	INIT(+,add);
	INIT(-,subtract);
	INIT(*,multiply);
	INIT(/,divide);
}
static inline double dub(obj_t *n)
{
	return n->type==INTEGER?(double)n->data.i:n->data.d;
}
FUNC_FROM_OP(+,add)
FUNC_FROM_OP(-,subtract)
FUNC_FROM_OP(*,multiply)
FUNC_FROM_OP(/,divide)
