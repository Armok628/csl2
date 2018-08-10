#include "types.h"
obj_t *new_obj(void)
{
	obj_t *obj=calloc(1,sizeof(obj_t));
	obj->refs=0;
	obj->type=ERROR;
	return obj;
}
obj_t *new_symbol(char *s)
{
	obj_t *o=new_obj();
	o->type=SYMBOL;
	o->data.sym=s;
	return o;
}
obj_t *new_integer(long i)
{
	obj_t *o=new_obj();
	o->type=INTEGER;
	o->data.i=i;
	return o;
}
obj_t *new_double(double d)
{
	obj_t *o=new_obj();
	o->type=DOUBLE;
	o->data.d=d;
	return o;
}
void destroy_func(obj_t *o)
{
	if (o->data.func.size<0) {
		free(o);
		return;
	}
	for (int i=0;i<o->data.func.size;i++)
		decr_refs(&o->data.func.rep.lisp[i]);
	free(o->data.func.rep.lisp);
	return;
}
void destroy(obj_t *o)
{
	printf("Destroying ");
	print(o);
	printf(" {%p}\n",(void *)o);
	switch (o->type) {
	case SYMBOL:
		free(o->data.sym);
		break;
	case CELL:
		decr_refs(o->data.cell.car);
		decr_refs(o->data.cell.cdr);
		break;
	case FUNCTION:
		destroy_func(o);
		break;
	case INTEGER:
	case DOUBLE:
	default:
		break;
	}
	free(o);
}
void incr_refs(obj_t *o)
{
	if (!o)
		return;
	o->refs+=o->refs>=0;
}
void decr_refs(obj_t *o)
{
	if (!o)
		return;
	o->refs-=o->refs>0;
	if (!o->refs)
		destroy(o);
}
obj_t *cons(obj_t *a,obj_t *b)
{
	obj_t *c=new_obj();
	c->type=CELL;
	c->data.cell.car=a;
	incr_refs(a);
	c->data.cell.cdr=b;
	incr_refs(b);
	return c;
}
void print_cell(obj_t *o)
{
	putchar('(');
	for (;o&&o->type==CELL;o=CDR(o)) {
		print(CAR(o));
		if (CDR(o))
			putchar(' ');
	}
	if (o) {
		putchar('.');
		putchar(' ');
		print(o);
	}
	putchar(')');
}
void print(obj_t *obj)
{
	if (!obj) {
		printf("NIL");
		return;
	}
	switch (obj->type) {
	case CELL:
		print_cell(obj);
		break;
	case INTEGER:
		printf("%ld",obj->data.i);
		break;
	case DOUBLE:
		printf("%f",obj->data.d);
		break;
	case SYMBOL:
		fputs(obj->data.sym,stdout);
		break;
	default:
		fputs("ERROR",stdout);
		break;
	}
}
