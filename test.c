#include <stdio.h>
#include <string.h>
#include "src/core.h"
#include "src/parser.h"
#include "src/types.h"
obj_t *sym(char *str)
{
	return new_symbol(strclone(str));
}
void puts_type(type_t t)
{
	switch (t) {
	case SYMBOL:
		puts("SYMBOL");
		break;
	case CELL:
		puts("CELL");
		break;
	case ERROR:
		puts("ERROR");
		break;
	case FUNCTION:
		puts("FUNCTION");
		break;
	case INTEGER:
		puts("INTEGER");
		break;
	case DOUBLE:
		puts("DOUBLE");
		break;
	default:
		puts("???");
		break;
	}
}
void test_infer_type(void)
{
	char buf[250];
	fgets(buf,250,stdin);
	char *tok=trim(buf);
	printf("Token read: '%s'\n",tok);
	puts_type(infer_type(tok));
}
void test_print(void)
{
	obj_t *o=cons(sym("a"),cons(sym("b"),NULL));
	incr_refs(o);
	print(o);
	putchar('\n');
	decr_refs(o);
}
void test_read(void)
{
	char buf[250];
	fgets(buf,250,stdin);
	obj_t *o=read(buf);
	fputs("Read: ",stdout);
	print(o);
	putchar('\n');
	puts_type(o->type);
	decr_refs(o);
}
int main(int argc,char **argv)
{
	dict=new_table(1000,(dtor_t)&decr_refs);
	char buf[250];
	for (;;) {
		fgets(buf,250,stdin);
		if (!strcmp(buf,"(quit)\n"))
			break;
		obj_t *s=read(buf);
		incr_refs(s);
		if (!strcmp(CAR(s)->data.sym,"set")) {
			print(set(CAR(CDR(s)),CAR(CDR(CDR(s)))));
		} else if (!strcmp(CAR(s)->data.sym,"get")) {
			print(get(CAR(CDR(s))));
		} else if (!strcmp(CAR(s)->data.sym,"unset")) {
			print(unset(CAR(CDR(s))));
		}
		putchar('\n');
		decr_refs(s);
	}
	free_table(dict);
}
