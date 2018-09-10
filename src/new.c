#include "new.h"
table_t *obtable=NULL;
obj_t *new_object(void)
{ // Returns uninitialized object with type ERROR
	obj_t *obj=malloc(sizeof(obj_t));
	if (!obj) {
		fputs("new_object: Failed to allocate memory",stderr);
		exit(1);
	}
	obj->refs=0;
	obj->type=ERROR;
	return obj;
}
obj_t *new_array(int size)
{
	obj_t *a=new_object();
	a->type=ARRAY;
	a->data.arr.size=size;
	a->data.arr.mem=calloc(size,sizeof(obj_t *));
	return a;
}
obj_t *new_cell(obj_t *a,obj_t *b)
{
	obj_t *c=new_object();
	c->type=CELL;
	CAR(c)=incr_refs(a);
	CDR(c)=incr_refs(b);
	return c;
}
obj_t *new_double(double d)
{
	obj_t *o=new_object();
	o->type=DOUBLE;
	o->data.d=d;
	return o;
}
obj_t *new_integer(long i)
{
	obj_t *o=new_object();
	o->type=INTEGER;
	o->data.i=i;
	return o;
}
obj_t *new_namespace(table_t *t)
{
	obj_t *o=new_object();
	o->type=NAMESPACE;
	o->data.table=t;
	return o;
}
obj_t *new_symbol(char *str)
{ // Returns identical symbol object if one already exists
	obj_t *o=lookup(obtable,str);
	// ^ Symbol table initialized by main
	if (o)
		return o;
	o=new_object();
	o->type=SYMBOL;
	o->data.sym=strdup(str);
	if (!o->data.sym) {
		fputs("new_symbol: Failed to allocate memory\n",stderr);
		exit(1);
	}
	insert(obtable,str,o);
	// ^ No incr_refs -- obtable has no destructor
	return o;
}
