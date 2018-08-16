#include "interpret.h"
obj_t *interpret_cond(obj_t *l)
{ // Returns list at COND_END
	l=CDR(l);
	bool ret=false;
	for (;!ret&&l&&!symbol_match(CAR(l),"COND_END");l=CDR(CDR(l))) {
		interpret(CAR(l));
		obj_t *r=pop();
		if (r) {
			interpret(CAR(CDR(l)));
			ret=true;
		}
		decr_refs(r);
	}
	if (!ret)
		push(NULL);
	while (l&&!symbol_match(CAR(l),"COND_END"))
		l=CDR(l);
	return l;
}
void interpret(obj_t *list)
{ // Requires RPN list as argument
	for (obj_t *o=list;o;o=CDR(o)) {
		if (!CAR(o)) {
			push(NULL);
		} else if (symbol_match(CAR(o),"LIST")) {
			push(CAR(o));
		} else if (symbol_match(CAR(o),"LIST_END")) {
			obj_t *body=NULL;
			while (!symbol_match(stack_obj(0),"LIST")) {
				obj_t *o=pop();
				if (o)
					o->refs--;
				body=new_cell(o,body);
			}
			drop();
			push(body);
		} else if (symbol_match(CAR(o),"DROP")) {
			drop();
		} else if (symbol_match(CAR(o),"CALL")) {
			obj_t *f=pop();
			if (!funcall(f))
				goto FATAL_INTERP_ERROR;
			decr_refs(f);
#ifdef ALL_ERRORS_FATAL
			obj_t *r=stack_obj(0);
			if (r&&r->type==ERROR)
				goto FATAL_INTERP_ERROR;
#endif
		} else if (symbol_match(CAR(o),"QUOTE")) {
			o=CDR(o);
			push(CAR(o));
		} else if (symbol_match(CAR(o),"COND")) {
			o=interpret_cond(o);
			// COND_END skipped by o=CDR(o) in for-loop
		} else if (symbol_match(CAR(o),"INSIDE")) {
			obj_t *env=pop();
			if (!type_check(env,NAMESPACE,"INSIDE: "))
				goto FATAL_INTERP_ERROR;
			push_namespace(env->data.table);
			obj_t *expr=pop();
			interpret(expr);
			decr_refs(expr);
			pop_namespace();
			decr_refs(env);
		} else if (CAR(o)->type==SYMBOL) {
			push(get_binding(CAR(o)));
		} else
			push(CAR(o));
	}
	return;

FATAL_INTERP_ERROR:
	fputs("Fatal error while interpreting ",stdout);
	print_obj(list);
	putchar('\n');
	print_stack();
	exit(1);
}

