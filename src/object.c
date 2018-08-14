#include "object.h"
const char *type_name(type_t type)
{
	switch (type) {
	case CELL:
		return "CELL";
	case DOUBLE:
		return "DOUBLE";
	case ERROR:
		return "ERROR";
	case HASHTABLE:
		return "HASHTABLE";
	case INTEGER:
		return "INTEGER";
	case FUNCTION:
		return "FUNCTION";
	case SYMBOL:
		return "SYMBOL";
	default:
		return "UNKNOWN";
	}
}
const char *obj_type_name(obj_t *obj)
{
	if (!obj)
		return "NIL";
	return type_name(obj->type);
}
bool type_check(obj_t *obj,type_t types,const char *err_prefix)
{ // Returns true if obj is of one of the types specified by type
	if (obj&&obj->type&types)
		return true;
	if (err_prefix)
		fputs(err_prefix,stdout);
	fputs("Expected ",stdout);
	for (int i=0;types>>i>0;i++) {
		if (!((types>>i)&1))
			continue;
		fputs(type_name(1<<i),stdout);
		if (types>>i>1)
			fputs(" or ",stdout);
	}
	if (obj) {
		printf(", but got %s: ",obj_type_name(obj));
		print_obj(obj);
		putchar('\n');
	} else
		puts(", but got NIL");

	return false;
}
void destroy(obj_t *o)
{ // Argument is assumed to be destroyable
	//printf("Destroying "); print_obj(o); printf(" {%p}\n",(void *)o);
	switch (o->type) {
	case SYMBOL:
		free(o->data.sym);
		break;
	case CELL:
		decr_refs(o->data.cell.car);
		decr_refs(o->data.cell.cdr);
		break;
	case FUNCTION:
		if (o->data.func.lambda)
			decr_refs(o->data.func.rep.lisp);
		break;
	case HASHTABLE:
		free_table(o->data.table);
		break;
	case INTEGER:
	case DOUBLE:
	default:
		break;
	}
	free(o);
}
obj_t *incr_refs(obj_t *o)
{
	if (!o)
		return NULL;
	o->refs++;
	return o;
}
void decr_refs(obj_t *o)
{
	if (!o)
		return;
	o->refs--;
	if (!o->refs)
		destroy(o);
}
void print_cell(obj_t *o)
{
	putchar('(');
	for (;o&&o->type==CELL;o=CDR(o)) {
		print_obj(CAR(o));
		if (CDR(o))
			putchar(' ');
	}
	if (o) {
		putchar('.');
		putchar(' ');
		print_obj(o);
	}
	putchar(')');
}
void print_obj(obj_t *obj)
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
	case ERROR:
		fputs("ERROR",stdout);
		break;
	default:
		printf("{%s:%p}",obj_type_name(obj),(void *)obj);
		break;
	}
	return;
}
bool eq_objs(obj_t *a,obj_t *b)
{
	if (a==b)
		return true;
	if (!a)
		return false;
	switch (a->type) {
	case SYMBOL:
		return !strcasecmp(a->data.sym,b->data.sym);
	case DOUBLE:
		return a->data.d==b->data.d;
	case INTEGER:
		return a->data.i==b->data.i;
	default:
		return false;
	}
}
bool symbol_match(obj_t *sym,const char *string)
{
	return sym&&sym->type==SYMBOL&&!strcasecmp(sym->data.sym,string);
}
int list_length(obj_t *list)
{
	int l=0;
	for (obj_t *o=list;o;o=CDR(o))
		l++;
	return l;
}
obj_t *copy_obj(obj_t *o)
{
	if (!o)
		return NULL;
	obj_t *c=new_object();
	c->type=o->type;
	switch (o->type) {
	case INTEGER:
		c->data.i=o->data.i;
		break;
	case DOUBLE:
		c->data.d=o->data.d;
		break;
	case CELL:
		c->data.cell.car=incr_refs(o->data.cell.car);
		c->data.cell.cdr=incr_refs(o->data.cell.cdr);
		break;
	case SYMBOL:
		c->data.sym=strdup(o->data.sym);
		break;
	default:
		return NULL;
	}
	return c;
}
void concatenate(obj_t *list,obj_t *item)
{
	for (;CDR(list);list=CDR(list));
	CDR(list)=incr_refs(item);
}
