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
			CDR(tail)=incr_refs(rpn(CAR(f)));
		}
		for (;CDR(tail);tail=CDR(tail));
		if (CDR(f)) {
			CDR(tail)=incr_refs(new_cell(strsym("DROP"),NULL));
			tail=CDR(tail);
		}
	}
	return list;
}
obj_t *translate_cond(obj_t *body)
{
	obj_t *list=new_cell(CAR(body),NULL);
	obj_t *tail=list;
	for (obj_t *c=CDR(body);c;c=CDR(c)) {
		obj_t *condition=CAR(CAR(c));
		obj_t *result=CAR(CDR(CAR(c)));
		CDR(tail)=incr_refs(new_cell(rpn(condition),NULL));
		tail=CDR(tail);
		CDR(tail)=incr_refs(new_cell(rpn(result),NULL));
		tail=CDR(tail);
	}
	CDR(tail)=incr_refs(new_cell(strsym("COND_END"),NULL));
	return list;
}
obj_t *translate_list(obj_t *body)
{
	obj_t *head=new_cell(CAR(body),NULL);
	obj_t *tail=head;
	for (obj_t *o=CDR(body);o;o=CDR(o)) {
		CDR(tail)=incr_refs(rpn(CAR(o)));
		for (;CDR(tail);tail=CDR(tail));
	}
	CDR(tail)=incr_refs(new_cell(strsym("LIST_END"),NULL));
	return head;
}
obj_t *translate_inside(obj_t *body)
{
	obj_t *l=new_cell(CAR(body),NULL);
	body=CDR(body);
	l=new_cell(CAR(body),l);
	body=CDR(body);
	l=new_cell(rpn(CAR(body)),l);
	return l;
}
obj_t *rpn(obj_t *body)
{
	if (!body||body->type!=CELL)
		return new_cell(body,NULL);
	if (symbol_match(CAR(body),"QUOTE"))
		return copy_obj(body);
	if (symbol_match(CAR(body),"COND"))
		return translate_cond(body);
	if (symbol_match(CAR(body),"PROGN"))
		return translate_progn(body);
	if (symbol_match(CAR(body),"LIST"))
		return translate_list(body);
	if (symbol_match(CAR(body),"INSIDE"))
		return translate_inside(body);
	obj_t *list=NULL;
	obj_t *tail=NULL;
	for (obj_t *o=CDR(body);o;o=CDR(o)) {
		if (!list) {
			list=rpn(CAR(o));
			tail=list;
		} else {
			CDR(tail)=incr_refs(rpn(CAR(o)));
		}
		for (;CDR(tail);tail=CDR(tail));
	}
	if (list) {
		CDR(tail)=incr_refs(rpn(CAR(body)));
	} else {
		list=rpn(CAR(body));
		tail=list;
	}
	for (;CDR(tail);tail=CDR(tail));
	CDR(tail)=incr_refs(new_cell(strsym("CALL"),NULL));
	return list;
}
