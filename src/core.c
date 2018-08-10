#include "core.h"
table_t *dict=NULL;
obj_t *cons(obj_t *a,obj_t *b)
{
	return new_cell(a,b);
}
obj_t *car(obj_t *c)
{
	if (c->type==CELL)
		return CAR(c);
	return new_obj();
}
obj_t *cdr(obj_t *c)
{
	if (c->type==CELL)
		return CDR(c);
	return new_obj();
}
obj_t *rplaca(obj_t *c,obj_t *v)
{
	if (c->type!=CELL)
		return new_obj();
	decr_refs(CAR(c));
	CAR(c)=v;
	incr_refs(v);
	return c;
}
obj_t *rplacd(obj_t *c,obj_t *v)
{
	if (c->type!=CELL)
		return new_obj();
	decr_refs(CDR(c));
	CDR(c)=v;
	incr_refs(v);
	return c;
}
obj_t *atom(obj_t *obj)
{
	return obj->type==CELL?NULL:obj;
}
obj_t *set(obj_t *sym,obj_t *val)
{
	if (sym->type!=SYMBOL)
		return new_obj();
	insert(dict,sym->data.sym,val);
	incr_refs(val);
	return sym;
}
obj_t *get(obj_t *sym)
{
	if (sym->type!=SYMBOL)
		return new_obj();
	return lookup(dict,sym->data.sym);
}
obj_t *unset(obj_t *sym)
{
	if (sym->type==SYMBOL)
		expunge(dict,sym->data.sym);
	return NULL;
}
