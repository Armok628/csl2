#include "core.h"
table_t *dict=NULL;
obj_t *set(obj_t *sym,obj_t *val)
{
	if (sym->type!=SYMBOL)
		return NULL;
	insert(dict,sym->data.sym,val);
	incr_refs(val);
	return sym;
}
obj_t *get(obj_t *sym)
{
	if (sym->type!=SYMBOL)
		return NULL;
	return lookup(dict,sym->data.sym);
}
obj_t *unset(obj_t *sym)
{
	if (sym->type==SYMBOL)
		expunge(dict,sym->data.sym);
	return NULL;
}
