#include "core.h"
void init_core(void)
{
	INIT(PRINT,print)
	INIT(TERPRI,terpri)
	INIT(CONS,cons)
	INIT(CAR,car)
	INIT(CDR,cdr)
	INIT(RPLACA,rplaca)
	INIT(RPLACD,rplacd)
	INIT(ATOM,atom)
	INIT(SET,set)
	INIT(GET,get)
	INIT(UNSET,unset)
	INIT(EQ,eq)
	INIT(COPY,copy)
	INIT(LENGTH,length)
	INIT(LAMBDA,lambda)
	INIT(EVAL,eval)
	INIT(READ,read)
	INIT(NULL,null)
	INIT(NOT,null)
	INIT(QUIT,quit)
	INIT(EXIT,quit)
	INIT(SEE,see)
	INIT(TICK,tick)
	INIT(TOCK,tock)
}
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
		return new_object();
	return CAR(c);
}
STACK(cdr,1_ARG)
obj_t *cdr(obj_t *c)
{
	if (!c)
		return NULL;
	if (!type_check(c,CELL,"CDR: "))
		return new_object();
	return CDR(c);
}
STACK(rplaca,2_ARGS)
obj_t *rplaca(obj_t *c,obj_t *v)
{
	if (!type_check(c,CELL,"RPLACA: "))
		return new_object();
	decr_refs(CAR(c));
	CAR(c)=incr_refs(v);
	return c;
}
STACK(rplacd,2_ARGS)
obj_t *rplacd(obj_t *c,obj_t *v)
{
	if (!type_check(c,CELL,"RPLACD: "))
		return new_object();
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
		return new_object();
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
		return new_object();
	return new_integer(list_length(list));
}
STACK(lambda,2_ARGS)
obj_t *lambda(obj_t *args,obj_t *body)
{
	if (!type_check(args,CELL,"LAMBDA ARGS: "))
		return new_object();
	for (obj_t *o=args;o;o=CDR(o))
		if (!type_check(CAR(o),SYMBOL,"LAMBDA ARG: "))
			return new_object();
	if (!type_check(body,CELL|SYMBOL,"LAMBDA BODY: "))
		return new_object();
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
	if (r)
		r->refs-=r->refs>0;
	return r;
}
STACK(read,1_ARG)
obj_t *read(obj_t *n)
{
	if (!type_check(n,INTEGER,"READ: "))
		return new_object();
	int l=n->data.i;
	char buf[l];
	fgets(buf,l,stdin);
	return read_str(buf);
}
STACK(null,1_ARG)
obj_t *null(obj_t *c)
{
	return !c?new_symbol(strdup("T")):NULL;
}
STACK(quit,0_ARGS)
obj_t *quit(void)
{
	exit(0);
}
STACK(see,1_ARG)
obj_t *see(obj_t *func)
{
	if (!type_check(func,FUNCTION,"SEE: "))
		return new_object();
	if (!func->data.func.lambda)
		return NULL;
	return CAR(CDR(CDR(func->data.func.rep.lisp)));
}
STACK(tick,0_ARGS)
obj_t *tick(void)
{
	start_timer();
	return NULL;
}
STACK(tock,0_ARGS)
obj_t *tock(void)
{
	return new_double(read_timer());
}
