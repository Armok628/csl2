#include "core.h"
obj_t *terpri(void)
{
	putchar('\n');
	return NULL;
}
STACK(terpri,0)
obj_t *cons(obj_t *a,obj_t *b)
{
	return new_cell(a,b);
}
STACK(cons,2)
obj_t *car(obj_t *c)
{
	if (!type_check(c,NIL|CELL,"CAR: "))
		return new_object();
	return c?CAR(c):NULL;
}
STACK(car,1)
obj_t *cdr(obj_t *c)
{
	if (!type_check(c,NIL|CELL,"CDR: "))
		return new_object();
	return c?CDR(c):NULL;
}
STACK(cdr,1)
obj_t *rplaca(obj_t *c,obj_t *v)
{
	if (!type_check(c,CELL,"RPLACA: "))
		return new_object();
	decr_refs(CAR(c));
	CAR(c)=incr_refs(v);
	return c;
}
STACK(rplaca,2)
obj_t *rplacd(obj_t *c,obj_t *v)
{
	if (!type_check(c,CELL,"RPLACD: "))
		return new_object();
	decr_refs(CDR(c));
	CDR(c)=incr_refs(v);
	return c;
}
STACK(rplacd,2)
obj_t *atom(obj_t *obj)
{
	return obj&&obj->type==CELL?NULL:T;
}
STACK(atom,1)
obj_t *set(obj_t *sym,obj_t *val)
{
	if (!type_check(sym,SYMBOL,"SET: "))
		return new_object();
	set_binding(sym->data.sym,val);
	return val;
}
STACK(set,2)
obj_t *get(obj_t *sym)
{
	if (!sym||sym->type!=SYMBOL)
		return sym;
	return get_binding(sym->data.sym);
}
STACK(get,1)
obj_t *unset(obj_t *sym)
{
	if (type_check(sym,SYMBOL,"UNSET: "))
		unset_binding(sym->data.sym);
	return NULL;
}
STACK(unset,1)
obj_t *eq(obj_t *a,obj_t *b)
{
	return eq_objs(a,b)?T:NULL;
}
STACK(eq,2)
obj_t *copy(obj_t *obj)
{
	return copy_cell(obj);
	// ^ Returns obj if obj->type!=CELL
}
STACK(copy,1)
obj_t *length(obj_t *coll)
{
	if (!type_check(coll,NIL|ARRAY|CELL,"LENGTH: "))
		return new_object();
	if (!coll||coll->type==CELL)
		return new_integer(list_length(coll));
	return new_integer(coll->data.arr.size);
}
STACK(length,1)
obj_t *lambda(obj_t *args,obj_t *body)
{
	bool type_err=false;
	type_err|=!type_check(args,NIL|CELL,"LAMBDA, args: ");
	for (obj_t *o=args;o;o=CDR(o))
		type_err|=!type_check(CAR(o),SYMBOL,"LAMBDA, arg: ");
	type_err|=!type_check(body,CELL,"LAMBDA, body: ");
	if (type_err)
		return new_object();
	return new_function(args,rpn(body));
}
STACK(lambda,2)
obj_t *eval(obj_t *expr)
{
	obj_t *r=incr_refs(rpn(expr));
	interpret(r);
	decr_refs(r);
	return dpop();
}
STACK(eval,1)
#define READ_BUF_LEN (80*24)
obj_t *lread()
{
	static char buf[READ_BUF_LEN];
	fgets(buf,READ_BUF_LEN,stdin);
	return read_str(buf);
}
STACK(lread,0)
obj_t *null(obj_t *c)
{
	return !c?T:NULL;
}
STACK(null,1)
obj_t *quit(void)
{
	exit(0);
}
STACK(quit,0)
obj_t *see(obj_t *func)
{
	if (!type_check(func,FUNCTION,"SEE: "))
		return new_object();
	if (!func->data.func.lambda)
		return NULL;
	return func->data.func.rep.lisp;
}
STACK(see,1)
static clock_t last_tick=-1;
obj_t *tick(void)
{
	last_tick=clock();
	if (last_tick==-1)
		return NULL;
	return T;
}
STACK(tick,0)
obj_t *tock(void)
{
	clock_t now=clock();
	if (now==-1)
		return NULL;
	return new_double((now-last_tick)/(double)CLOCKS_PER_SEC);
}
STACK(tock,0)
obj_t *uplevel(obj_t *n,obj_t *expr)
{ // Evaluate expr in namespace n callers up
	bool type_err=false;
	type_err|=!type_check(n,INTEGER,"UPLEVEL, arg 1: ");
	type_err|=!type_check(expr,CELL|SYMBOL,"UPLEVEL, arg 2: ");
	if (type_err)
		return new_object();
	if (level<1) {
		puts("UPLEVEL: No higher level");
		return new_object();
	}
	table_t *loc=pop_namespace();
	obj_t *t=incr_refs(rpn(expr));
	interpret(t);
	decr_refs(t);
	push_namespace(loc);
	return dpop();
}
STACK(uplevel,2)
obj_t *nconc(obj_t *a,obj_t *b)
{
	if (!type_check(a,CELL,"NCONC: "))
		return new_object();
	concatenate(a,b);
	return a;
}
STACK(nconc,2)
obj_t *lfor(obj_t *init,obj_t *cond,obj_t *iter,obj_t *body)
{
	bool type_err=false;
	type_err|=!type_check(init,NIL|CELL,"FOR, initialization: ");
	type_err|=!type_check(init,NIL|CELL,"FOR, condition: ");
	type_err|=!type_check(iter,NIL|CELL,"FOR, iteration: ");
	type_err|=!type_check(body,CELL,"FOR, body: ");
	if (type_err)
		return new_object();
	init=incr_refs(rpn(init));
	cond=incr_refs(rpn(cond));
	iter=incr_refs(rpn(iter));
	body=incr_refs(rpn(body));
	interpret(init);
	decr_refs(init);
	drop();
	obj_t *ret=NULL;
	obj_t *c;
	for (;;) {
		if (cond) {
			interpret(cond);
			c=pop();
			if (c)
				decr_refs(c);
			else
				break;
		}
		interpret(body);
		decr_refs(ret);
		ret=pop();
		interpret(iter);
		drop();
	}
	decr_refs(cond);
	decr_refs(body);
	decr_refs(iter);
	if (ret)
		ret->refs--;
	return ret;
}
STACK(lfor,4)
obj_t *foreach_in_list(obj_t *name,obj_t *list,obj_t *sbe)
{
	obj_t *ret=NULL;
	for (obj_t *o=list;o;o=CDR(o)) {
		set_binding(name->data.sym,CAR(o));
		interpret(sbe);
		decr_refs(ret);
		ret=pop();
	}
	if (ret)
		ret->refs--;
	return ret;
}
obj_t *foreach_in_arr(obj_t *name,obj_t *arr,obj_t *sbe)
{
	obj_t *ret=NULL;
	int size=arr->data.arr.size;
	for (int i=0;i<size;i++) {
		set_binding(name->data.sym,arr->data.arr.mem[i]);
		interpret(sbe);
		decr_refs(ret);
		ret=pop();
	}
	if (ret)
		ret->refs--;
	return ret;
}
obj_t *foreach(obj_t *name,obj_t *coll,obj_t *body)
{ // Bind each collection object to name in current context and eval body
	bool type_err=false;
	type_err|=!type_check(name,SYMBOL,"FOREACH, name: ");
	type_err|=!type_check(coll,NIL|ARRAY|CELL,"FOREACH, collection: ");
	type_err|=!type_check(body,CELL,"FOREACH, body: ");
	if (type_err)
		return new_object();
	if (!coll)
		return NULL;
	body=incr_refs(rpn(body));
	obj_t *ret;
	if (!coll||coll->type==CELL)
		ret=foreach_in_list(name,coll,body);
	else
		ret=foreach_in_arr(name,coll,body);
	decr_refs(body);
	return ret;
}
STACK(foreach,3)
obj_t *load(obj_t *file)
{ // Returns an object as if contents were given to READ
	if (!type_check(file,SYMBOL,"LOAD: "))
		return new_object();
	obj_t *r=load_file(file->data.sym);
	return r?r:new_object();
}
STACK(load,1)
obj_t *namespace(void)
{
	return new_namespace(new_table(NAMESPACE_SIZE,(dtor_t)&decr_refs));
}
STACK(namespace,0)
obj_t *inside(obj_t *namespace,obj_t *expr)
{ // Evaluate expr inside namespace
	bool type_err=false;
	type_err|=!type_check(namespace,NAMESPACE,"INSIDE, namespace: ");
	type_err|=!type_check(expr,CELL|SYMBOL,"INSIDE, expression: ");
	if (type_err)
		return new_object();
	obj_t *t=incr_refs(rpn(expr));
	push_namespace(namespace->data.table);
	interpret(t);
	decr_refs(t);
	pop_namespace();
	return dpop();
}
STACK(inside,2)
obj_t *l_typeof(obj_t *obj)
{ // Returns interned symbol
	return new_symbol(obj_type_name(obj));
}
STACK(l_typeof,1)
obj_t *append(obj_t *a,obj_t *b)
{
	if (!type_check(a,CELL,"APPEND: "))
		return new_object();
	a=copy_cell(a);
	concatenate(a,b);
	return a;
}
STACK(append,2)
obj_t *l_interpret(obj_t *sbe)
{ // Takes a stack-based expression
	interpret(sbe);
	return NULL;
}
STACK(l_interpret,1)
obj_t *stack(void)
{ // Returns number of stack items
	return new_integer(print_stack());
}
STACK(stack,0)
obj_t *array(obj_t *n)
{
	if (!type_check(n,INTEGER,"ARRAY: "))
		return new_object();
	return new_array(n->data.i);
}
STACK(array,1)
obj_t *aget(obj_t *a,obj_t *n)
{
	bool type_err=false;
	type_err|=!type_check(a,ARRAY,"AGET, array: ");
	type_err|=!type_check(n,INTEGER,"AGET, index: ");
	if (type_err)
		return new_object();
	if (n->data.i>=a->data.arr.size) {
		fputs("AGET: Out of bounds\n",stderr);
		return new_object();
	}
	return a->data.arr.mem[n->data.i];
}
STACK(aget,2)
obj_t *aset(obj_t *a,obj_t *n,obj_t *o)
{
	bool type_err=false;
	type_err|=!type_check(a,ARRAY,"ASET, array: ");
	type_err|=!type_check(n,INTEGER,"ASET, index: ");
	if (type_err)
		return new_object();
	if (n->data.i>=a->data.arr.size) {
		fputs("ASET: Out of bounds\n",stderr);
		return new_object();
	}
	decr_refs(a->data.arr.mem[n->data.i]);
	a->data.arr.mem[n->data.i]=incr_refs(o);
	return o;
}
STACK(aset,3)
void init_core(void)
{
	insert(dict,"T",incr_refs(T));
	insert(dict,"\\n",incr_refs(&newline_sym));
	insert(dict,"\\t",incr_refs(&tab_sym));
	INIT(ARRAY,array)
	INIT(AGET,aget)
	INIT(APPEND,append)
	INIT(ASET,aset)
	INIT(ATOM,atom)
	INIT(CAR,car)
	INIT(CDR,cdr)
	INIT(CONS,cons)
	INIT(COPY,copy)
	INIT(EQ,eq)
	INIT(EVAL,eval)
	INIT(EXIT,quit)
	INIT(FOR,lfor)
	INIT(FOREACH,foreach)
	INIT(GET,get)
	INIT(INSIDE,inside)
	INIT(INTERPRET,l_interpret)
	INIT(LAMBDA,lambda)
	INIT(LENGTH,length)
	INIT(LOAD,load)
	INIT(NAMESPACE,namespace)
	INIT(NCONC,nconc)
	INIT(NOT,null)
	INIT(NULL,null)
	INIT(QUIT,quit)
	INIT(READ,lread)
	INIT(RPLACA,rplaca)
	INIT(RPLACD,rplacd)
	INIT(SEE,see)
	INIT(SET,set)
	INIT(STACK,stack)
	INIT(TERPRI,terpri)
	INIT(TICK,tick)
	INIT(TOCK,tock)
	INIT(TYPEOF,l_typeof)
	INIT(UNSET,unset)
	INIT(UPLEVEL,uplevel)
}
