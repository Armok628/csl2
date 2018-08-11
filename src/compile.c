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
	if (!body||body->type!=CELL)
		return new_cell(body,NULL);
	if (CAR(body)->type==SYMBOL&&!strcmp(CAR(body)->data.sym,"QUOTE"))
		return CDR(body);
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
	CDR(tail)=rpn(CAR(body));
	incr_refs(CDR(tail));
	tail=CDR(tail);
	CDR(tail)=new_cell(new_symbol(strdup("CALL")),NULL);
	incr_refs(CDR(tail));
	return list;
}
