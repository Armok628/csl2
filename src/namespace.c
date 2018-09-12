#include "namespace.h"
table_t *dict=NULL;
table_t *namespaces[STACK_SIZE];
int level=-1; // Last filled slot
void init_dict(void)
{
	dict=new_table(NAMESPACE_SIZE,(dtor_t)&decr_refs);
	push_namespace(dict);
	// Miscellaneous dictionary setup tasks here:
	hash_function=&nocase_fnv_1a;
	insert(dict,"DICTIONARY",incr_refs(new_namespace(dict)));
	insert(dict,"T",incr_refs(T));
	insert(dict,"\\n",incr_refs(&newline_sym));
	insert(dict,"\\t",incr_refs(&tab_sym));
	insert(dict,"STDOUT",incr_refs(new_filestream(stdout)));
	insert(dict,"STDERR",incr_refs(new_filestream(stderr)));
	insert(dict,"ERROR",incr_refs(error));
	// Calls to init functions go here:
	init_core();
	init_arith();
	init_math();
	init_example(); // See example.c
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
