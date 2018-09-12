#include "compile.h"
obj_t *translate_progn(obj_t *body)
{ // (progn a b c) => (rpn(a) DROP rpn(b) DROP rpn(c))
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
			CDR(tail)=incr_refs(LIST1(&drop_sym));
			tail=CDR(tail);
		}
	}
	return list;
}
obj_t *translate_cond(obj_t *body)
{ // (COND (a b) (c d)) => (COND (rpn(a)) (rpn(b)) (rpn(c)) (rpn(d)) COND_END)
	obj_t *list=LIST1(CAR(body));
	obj_t *tail=list;
	for (obj_t *c=CDR(body);c;c=CDR(c)) {
		obj_t *condition=CAR(CAR(c));
		obj_t *result=CAR(CDR(CAR(c)));
		CDR(tail)=incr_refs(LIST2(rpn(condition),rpn(result)));
		tail=CDR(CDR(tail));
	}
	CDR(tail)=incr_refs(LIST1(&cond_end_sym));
	return list;
}
obj_t *translate_list(obj_t *body)
{ // (LIST a b c d) => (LIST rpn(a) rpn(b) rpn(c) rpn(d) LIST_END)
	obj_t *head=LIST1(CAR(body));
	obj_t *tail=head;
	for (obj_t *o=CDR(body);o;o=CDR(o)) {
		CDR(tail)=incr_refs(rpn(CAR(o)));
		for (;CDR(tail);tail=CDR(tail));
	}
	CDR(tail)=incr_refs(LIST1(&list_end_sym));
	return head;
}
obj_t *translate_if(obj_t *body)
{
	// (IF a b c) => (COND (rpn(a)) (rpn(b)) (T) (rpn(c)) COND_END)
	// (IF a b) => (COND (rpn(a)) (rpn(b)) COND_END)
	obj_t *list=LIST1(&cond_sym);
	obj_t *tail=list;
	body=CDR(body);
	obj_t *condition=CAR(body);
	body=CDR(body);
	obj_t *if_result=CAR(body);
	body=CDR(body);
	obj_t *else_result=body?CAR(body):NULL;
	CDR(tail)=incr_refs(LIST1(rpn(condition)));
	tail=CDR(tail);
	CDR(tail)=incr_refs(LIST1(rpn(if_result)));
	tail=CDR(tail);
	if (else_result) {
		CDR(tail)=incr_refs(LIST2(LIST1(T),rpn(else_result)));
		tail=CDR(CDR(tail));
	}
	CDR(tail)=incr_refs(LIST1(&cond_end_sym));
	return list;
}
obj_t *translate_print(obj_t *body)
{ // (PRINT a b c) => (rpn(a) PRINT rpn(b) PRINT rpn(c) PRINT NIL)
	obj_t *list=NULL;
	obj_t *tail=NULL;
	for (obj_t *o=CDR(body);o;o=CDR(o)) {
		if (!list) {
			list=rpn(CAR(o));
			tail=list;
		} else
			CDR(tail)=incr_refs(rpn(CAR(o)));
		for (;CDR(tail);tail=CDR(tail));
		CDR(tail)=incr_refs(LIST1(&print_sym));
		tail=CDR(tail);
	}
	if (tail)
		CDR(tail)=incr_refs(LIST2(&print_end_sym,NULL));
	return list;
}
obj_t *rpn(obj_t *body)
{ // (a b c d) => (rpn(b) rpn(c) rpn(d) rpn(a) EXECUTE)
	if (!body||body->type!=CELL)
		return LIST1(body);
	if (CAR(body)==&quote_sym)
		return copy_cell(body);
	if (CAR(body)==&cond_sym)
		return translate_cond(body);
	if (CAR(body)==&print_sym)
		return translate_print(body);
	if (CAR(body)==&progn_sym)
		return translate_progn(body);
	if (CAR(body)==&list_sym)
		return translate_list(body);
	if (CAR(body)==&if_sym)
		return translate_if(body);
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
	CDR(tail)=incr_refs(LIST1(&execute_sym));
	return list;
}
