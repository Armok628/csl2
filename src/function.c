#include "function.h"
// New C functions are made available to the language
// via static variables. See STACK in core.h
obj_t *new_function(obj_t *argl,obj_t *body)
{
	obj_t *func=new_object();
	func->type=FUNCTION;
	func->data.func.lambda=true;
	obj_t *rep=CONS(argl,CONS(body,NULL));
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
	table_t *local=new_table(LOCAL_NAMESPACE_SIZE,(dtor_t)&decr_refs);
	insert(local,"RECURSE",incr_refs(func));
	push_namespace(local);
	bind_args(args);
	interpret(body);
	drop_namespace();
	return true;
}
