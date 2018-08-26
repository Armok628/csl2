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
	case FUNCTION:
		return "FUNCTION";
	case NAMESPACE:
		return "NAMESPACE";
	case INTEGER:
		return "INTEGER";
	case NIL:
		return "NIL"; // For use by type_check error messages
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
{ // Returns true if obj is of one of the types specified by bitmask type
	if ((obj&&obj->type&types)||(!obj&&types&NIL))
		return true;
	if (err_prefix)
		fputs(err_prefix,stderr);
	fputs("Expected ",stderr);
	for (int i=0;types>>i>0;i++) {
		if (!((types>>i)&1))
			continue;
		fputs(type_name(1<<i),stderr);
		if (types>>i>1)
			fputs(" or ",stderr);
	}
	if (obj) {
		fprintf(stderr,", but got %s: ",obj_type_name(obj));
		print_obj(obj,stderr);
		fputc('\n',stderr);
	} else
		fputs(", but got NIL\n",stderr);
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
		decr_refs(CAR(o));
		decr_refs(CDR(o));
		break;
	case FUNCTION:
		if (o->data.func.lambda)
			decr_refs(o->data.func.rep.lisp);
		break;
	case NAMESPACE:
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
void print_cell(obj_t *o,FILE *fs)
{
	fputc('(',fs);
	for (;o&&o->type==CELL;o=CDR(o)) {
		print_obj(CAR(o),fs);
		if (CDR(o))
			fputc(' ',fs);
	}
	if (o) {
		fputc('.',fs);
		fputc(' ',fs);
		print_obj(o,fs);
	}
	fputc(')',fs);
}
void print_obj(obj_t *obj,FILE *fs)
{
	if (!obj) {
		fprintf(fs,"NIL");
		return;
	}
	switch (obj->type) {
	case CELL:
		print_cell(obj,fs);
		break;
	case INTEGER:
		fprintf(fs,"%ld",obj->data.i);
		break;
	case DOUBLE:
		fprintf(fs,"%f",obj->data.d);
		break;
	case SYMBOL:
		fputs(obj->data.sym,fs);
		break;
	case ERROR:
		fputs("ERROR",fs);
		break;
	default:
		fprintf(fs,"{%s:%p}",obj_type_name(obj),(void *)obj);
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
	switch (o->type) {
	case INTEGER:
		return new_integer(o->data.i);
	case DOUBLE:
		return new_double(o->data.d);
	case CELL:
		return new_cell(copy_obj(CAR(o)),copy_obj(CDR(o)));
	case SYMBOL:
		return new_symbol(o->data.sym);
	default:
		return new_object();
	}
}
void concatenate(obj_t *list,obj_t *item)
{
	for (;CDR(list);list=CDR(list));
	CDR(list)=incr_refs(item);
}
