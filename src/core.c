#include "core.h"
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
