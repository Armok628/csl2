#include "compile.h"
obj_t *translate_progn(obj_t *body)
{
	obj_t *list=NULL;
	obj_t *tail=NULL;
	for (obj_t *f=CDR(body);f;f=CDR(f)) {
		if (!list) {
			list=rpn(CAR(f));
			tail=list;
		} else {
			CDR(tail)=rpn(CAR(f));
		}
		for (;CDR(tail);tail=CDR(tail));
		if (CDR(f)) {
			CDR(tail)=new_cell(new_symbol(strdup("DROP")),NULL);
			tail=CDR(tail);
		}
	}
	return list;
}
obj_t *rpn(obj_t *body)
{
	if (!body||body->type!=CELL)
		return new_cell(body,NULL);
	if (symbol_match(CAR(body),"QUOTE"))
		return body;
	if (symbol_match(CAR(body),"COND"))
		return body;
	if (symbol_match(CAR(body),"PROGN"))
		return translate_progn(body);
	obj_t *list=NULL;
	obj_t *tail=NULL;
	for (obj_t *o=CDR(body);o;o=CDR(o)) {
		if (!list) {
			list=rpn(CAR(o));
			tail=list;
		} else {
			CDR(tail)=rpn(CAR(o));
			incr_refs(CDR(tail));
		}
		for (;CDR(tail);tail=CDR(tail));
	}
	if (list) {
		CDR(tail)=rpn(CAR(body));
		incr_refs(CDR(tail));
		tail=CDR(tail);
	} else {
		list=rpn(CAR(body));
		tail=list;
	}
	CDR(tail)=new_cell(new_symbol(strdup("CALL")),NULL);
	incr_refs(CDR(tail));
	return list;
}
