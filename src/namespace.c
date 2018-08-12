#include "namespace.h"
// TODO: Do I want an equivalent to Tcl's upvar/uplevel?
table_t *namespaces[STACK_SIZE];
int level=-1; // Unlike stack_index, level is last *filled* slot
void push_namespace(table_t *env)
{
	namespaces[++level]=env;
}
table_t *pop_namespace(void)
{
	return namespaces[level--];
}
void drop_namespace(void)
{
	free_table(namespaces[level--]);
}
table_t *new_namespace(void)
{
	return new_table(STACK_SIZE,(dtor_t)&decr_refs);
}
obj_t *get_binding(obj_t *sym)
{
	obj_t *l=lookup(namespaces[level],sym->data.sym);
	if (!l)
		l=lookup(dict,sym->data.sym);
	return l;
}
void set_binding(obj_t *sym,obj_t *val)
{
	incr_refs(val);
	insert(namespaces[level],sym->data.sym,val);
}
void unset_binding(obj_t *sym)
{
	expunge(namespaces[level],sym->data.sym);
}
