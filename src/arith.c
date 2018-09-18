#include "arith.h"
static inline double obj2double(obj_t *n)
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
	return new_double(obj2double(x) op obj2double(y)); \
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
	return obj2double(x) op obj2double(y)?T:NULL; \
} \
STACK(name,2)
FUNC_FROM_COMP(>,gt)
FUNC_FROM_COMP(>=,gte)
FUNC_FROM_COMP(<,lt)
FUNC_FROM_COMP(<=,lte)
FUNC_FROM_COMP(==,eqn)
obj_t *or(obj_t *x,obj_t *y)
{
	return x||y?T:NULL;
}
STACK(or,2)
obj_t *and(obj_t *x,obj_t *y)
{
	return x&&y?T:NULL;
}
STACK(and,2)
obj_t *xor(obj_t *x,obj_t *y)
{
	return !x^!y?T:NULL;
}
STACK(xor,2)
obj_t *mod(obj_t *x,obj_t *y)
{
	bool type_err=false;
	type_err|=!type_check(x,INTEGER|DOUBLE,"MOD, arg 1: ");
	type_err|=!type_check(y,INTEGER|DOUBLE,"MOD, arg 2: ");
	if (type_err)
		return error;
	if (x->type==INTEGER&&y->type==INTEGER)
		return new_integer(x->data.i%y->data.i);
	return new_double(fmod(obj2double(x),obj2double(y)));
}
STACK(mod,2)
obj_t *l_float(obj_t *x)
{
	if (!type_check(x,INTEGER,"FLOAT: "))
		return error;
	return new_double(x->data.i);
}
STACK(l_float,1)
obj_t *incr(obj_t *var,obj_t *amt)
{
	bool type_err=false;
	type_err|=!type_check(var,SYMBOL,"+=, name: ");
	type_err|=!type_check(amt,DOUBLE|INTEGER,"+=, increment: ");
	if (type_err)
		return error;
	obj_t *o=incr_refs(get_binding(var->data.sym));
	if (o==error) {
		set_binding(var->data.sym,amt);
		decr_refs(o);
		return amt;
	}
	if (!type_check(o,DOUBLE|INTEGER,"+=, value: "))
		return o;
	obj_t *n;
	if (o->type==DOUBLE)
		if (amt->type==DOUBLE)
			n=new_double(o->data.d+amt->data.d);
		else
			n=new_double(o->data.d+amt->data.i);
	else
		if (amt->type==DOUBLE)
			n=new_double(o->data.i+amt->data.d);
		else
			n=new_integer(o->data.i+amt->data.i);
	decr_refs(o);
	set_binding(var->data.sym,n);
	return n;
}
STACK(incr,2)
void init_arith(void)
{
	srand(time(NULL));
	INIT(RANDOM,l_random)
	INIT(+,add)
	INIT(-,subtract)
	INIT(*,multiply)
	INIT(/,divide)
	INIT(<,lt)
	INIT(<=,lte)
	INIT(==,eqn)
	INIT(>,gt)
	INIT(>=,gte)
	INIT(AND,and)
	INIT(OR,or)
	INIT(XOR,xor)
	INIT(MOD,mod)
	INIT(FLOAT,l_float)
	INIT(+=,incr)
}
