#include "compile.h"
obj_t *compile(obj_t *list,int size)
{
	obj_t *mem=calloc(size,sizeof(obj_t));
	for (int i=0;i<size;i++) {
		mem[i]=*list;
		incr_refs(list);
		list=CDR(list);
	}
	return mem;
}
obj_t *rpn(obj_t *body)
{
	if (body->type!=CELL)
		return cons(body,NULL);
	obj_t *list=NULL;
	obj_t **tail=NULL;
	for (obj_t *o=CDR(body);o;o=CDR(o)) {
		if (!list) {
			list=rpn(CAR(o));
			tail=&CDR(list);
		} else {
			*tail=rpn(CAR(o));
		}
		for (;*tail;tail=&CDR(*tail));
	}
	*tail=rpn(CAR(body));
	return list;
}
