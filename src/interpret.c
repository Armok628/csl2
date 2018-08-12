#include "interpret.h"
// These functions are prototypes
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
	while (!symbol_match(CAR(l),"COND_END"))
		l=CDR(l);
	return l;
}
void interpret(obj_t *list)
{
	for (obj_t *o=list;o;o=CDR(o)) {
		if (symbol_match(CAR(o),"DROP")) {
			drop();
		} else if (symbol_match(CAR(o),"CALL")) {
			obj_t *f=pop();
			f->data.func.rep.c();
			decr_refs(f);
		} else if (symbol_match(CAR(o),"QUOTE")) {
			o=CDR(o);
			push(CAR(o));
			continue;
		} else if (symbol_match(CAR(o),"COND")) {
			o=interpret_cond(o);
			// COND_END skipped by o=CDR(o) in for-loop
		} else
			push(get(CAR(o)));
	}
}
