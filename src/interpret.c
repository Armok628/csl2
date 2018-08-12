#include "interpret.h"
void interpret(obj_t *list) // Prototype
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
		} else
			push(get(CAR(o)));
	}
}
