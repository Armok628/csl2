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
	obj_t *rep=NULL;
	rep=new_cell(body,rep);
	rep=new_cell(argl,rep);
	table_t *t=new_namespace();
	insert(t,"RECURSE",incr_refs(func));
	rep=new_cell(new_hashtable(t),rep);
	func->data.func.rep.lisp=rep;
	return func;
}
void bind_args(obj_t *argl)
{
	int l=list_length(argl);
	for (int i=l-1;i>=0;i--) {
		set_binding(CAR(argl),stack_obj(i));
		argl=CDR(argl);
	}
	for (int i=0;i<l;i++)
		drop();
}
void funcall(obj_t *func)
{
	if (!type_check(func,FUNCTION,"Fatal: funcall(): "))
		exit(1);
	if (!func->data.func.lambda) {
		func->data.func.rep.c();
		return;
	}
	obj_t *rep=func->data.func.rep.lisp;
	obj_t *local_env=CAR(rep);
	rep=CDR(rep);
	obj_t *args=CAR(rep);
	rep=CDR(rep);
	obj_t *body=CAR(rep);
	push_namespace(local_env->data.table);
	if (args)
		bind_args(args);
	interpret(body);
	pop_namespace();
}
