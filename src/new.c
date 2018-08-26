#include "new.h"
obj_t *new_object(void)
{ // Returns uninitialized object with type ERROR
	obj_t *obj=calloc(1,sizeof(obj_t));
	obj->refs=0;
	obj->type=ERROR;
	return obj;
}
obj_t *new_cell(obj_t *a,obj_t *b)
{
	obj_t *c=new_object();
	c->type=CELL;
	CAR(c)=incr_refs(a);
	CDR(c)=incr_refs(b);
	return c;
}
obj_t *new_symbol(char *str)
{ // Duplicates str
	obj_t *o=new_object();
	o->type=SYMBOL;
	o->data.sym=strdup(str);
	return o;
}
obj_t *new_integer(long i)
{
	obj_t *o=new_object();
	o->type=INTEGER;
	o->data.i=i;
	return o;
}
obj_t *new_double(double d)
{
	obj_t *o=new_object();
	o->type=DOUBLE;
	o->data.d=d;
	return o;
}
obj_t *new_namespace_obj(table_t *table)
{
	obj_t *o=new_object();
	o->type=NAMESPACE;
	o->data.table=table;
	return o;
}
