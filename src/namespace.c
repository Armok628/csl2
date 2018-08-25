#include "namespace.h"
table_t *dict=NULL;
table_t *namespaces[STACK_SIZE];
int level=-1; // Unlike stack_index, level is last *filled* slot
void init_dict(void)
{
	dict=new_namespace_table(NAMESPACE_SIZE);
	push_namespace(dict);
	// Miscellaneous dictionary setup tasks here:
	hash_function=&nocase_hash_key;
	insert(dict,"DICTIONARY",incr_refs(new_namespace_obj(dict)));
	// Calls to init functions go here:
	init_core();
	init_arith();
}
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
table_t *new_namespace_table(int size)
{
	return new_table(size,(dtor_t)&decr_refs);
}
obj_t *get_binding(char *str)
{
	obj_t *l=lookup(namespaces[level],str);
	if (!l)
		l=lookup(dict,str);
	return l;
}
void set_binding(char *str,obj_t *val)
{
	insert(namespaces[level],str,incr_refs(val));
}
void unset_binding(char *str)
{
	expunge(namespaces[level],str);
}
