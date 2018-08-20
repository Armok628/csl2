#include "function.h"
obj_t *new_cfunction(void (*f)(void))
{
	obj_t *o=new_object();
	o->type=FUNCTION;
	o->data.func.lambda=false;
	o->data.func.rep.c=f;
	return o;
}
obj_t *new_lispfunction(obj_t *argl,obj_t *body)
{
	obj_t *func=new_object();
	func->type=FUNCTION;
	func->data.func.lambda=true;
	obj_t *rep=new_cell(argl,new_cell(body,NULL));
	func->data.func.rep.lisp=incr_refs(rep);
	return func;
}
void bind_args(obj_t *argl)
{
	int l=list_length(argl);
	for (int i=l-1;i>=0;i--) {
		set_binding(CAR(argl)->data.sym,stack_obj(i));
		argl=CDR(argl);
	}
	for (int i=0;i<l;i++)
		drop();
}
bool funcall(obj_t *func)
{ // Returns true if no fatal error occurred
	if (!type_check(func,FUNCTION,"funcall(): "))
		return false;
	if (!func->data.func.lambda) {
		func->data.func.rep.c();
		return true;
	}
	obj_t *rep=func->data.func.rep.lisp;
	obj_t *args=CAR(rep);
	obj_t *body=CAR(CDR(rep));
	table_t *loc=new_namespace_table(1);
	insert(loc,"RECURSE",incr_refs(func));
	push_namespace(loc);
	bind_args(args);
	interpret(body);
	drop_namespace();
	return true;
}
