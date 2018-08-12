#include "core.h"
table_t *dict=NULL;
STACK(print,1_ARG)
obj_t *print(obj_t *obj)
{
	print_obj(obj);
	return obj;
}
STACK(terpri,0_ARGS)
obj_t *terpri(void)
{
	putchar('\n');
	return NULL;
}
STACK(cons,2_ARGS)
obj_t *cons(obj_t *a,obj_t *b)
{
	return new_cell(a,b);
}
STACK(car,1_ARG)
obj_t *car(obj_t *c)
{
	if (!c)
		return NULL;
	if (!type_check(c,CELL,"CAR: "))
		return new_obj();
	return CAR(c);
}
STACK(cdr,1_ARG)
obj_t *cdr(obj_t *c)
{
	if (!c)
		return NULL;
	if (!type_check(c,CELL,"CDR: "))
		return CDR(c);
	return new_obj();
}
STACK(rplaca,2_ARGS)
obj_t *rplaca(obj_t *c,obj_t *v)
{
	if (!type_check(c,CELL,"RPLACA: "))
		return new_obj();
	decr_refs(CAR(c));
	CAR(c)=incr_refs(v);
	return c;
}
STACK(rplacd,2_ARGS)
obj_t *rplacd(obj_t *c,obj_t *v)
{
	if (!type_check(c,CELL,"RPLACD: "))
		return new_obj();
	decr_refs(CDR(c));
	CDR(c)=incr_refs(v);
	return c;
}
STACK(atom,1_ARG)
obj_t *atom(obj_t *obj)
{
	if (!obj)
		return new_symbol(strdup("T"));
	return obj->type==CELL?NULL:obj;
}
STACK(set,2_ARGS)
obj_t *set(obj_t *sym,obj_t *val)
{
	if (!type_check(sym,SYMBOL,"SET: "))
		return new_obj();
	set_binding(sym,val);
	return sym;
}
STACK(get,1_ARG)
obj_t *get(obj_t *sym)
{
	if (!sym||sym->type!=SYMBOL)
		return sym;
	return get_binding(sym);
}
STACK(unset,1_ARG)
obj_t *unset(obj_t *sym)
{
	if (!sym||sym->type==SYMBOL)
		unset_binding(sym);
	return NULL;
}
STACK(eq,2_ARGS)
obj_t *eq(obj_t *a,obj_t *b)
{
	if (eq_objs(a,b))
		return new_symbol(strdup("T"));
	else
		return NULL;
}
STACK(copy,1_ARG)
obj_t *copy(obj_t *obj)
{
	return copy_obj(obj);
}
STACK(length,1_ARG)
obj_t *length(obj_t *list)
{
	if (!type_check(list,CELL,"LENGTH: "))
		return new_obj();
	return new_integer(list_length(list));
}
STACK(lambda,2_ARGS)
obj_t *lambda(obj_t *args,obj_t *body)
{
	if (!type_check(args,CELL,"LAMBDA ARGS: "))
		return new_obj();
	for (obj_t *o=args;o;o=CDR(o))
		if (!type_check(CAR(o),SYMBOL,"LAMBDA ARG: "))
			return new_obj();
	if (!type_check(body,CELL|SYMBOL,"LAMBDA BODY: "))
		return new_obj();
	return new_lispfunction(args,rpn(body));
}
STACK(eval,1_ARG)
obj_t *eval(obj_t *expr)
{
	obj_t *r=rpn(expr);
	incr_refs(r);
	interpret(r);
	decr_refs(r);
	r=pop();
	r->refs-=r->refs>0;
	return r;
}
