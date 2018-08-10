#include <stdio.h>
#include <string.h>
#include "src/core.h"
#include "src/compile.h"
#include "src/parser.h"
#include "src/types.h"
obj_t *sym(const char *str)
{
	return new_symbol(strdup(str));
}
void puts_type(obj_t *o)
{
	if (!o) {
		puts("NIL");
		return;
	}
	puts(type_names[o->type]);
}
void test_infer_type(void)
{
	char buf[250];
	fgets(buf,250,stdin);
	char *tok=trim(buf);
	printf("Token read: '%s'\n",tok);
	puts(type_names[infer_type(tok)]);
}
void test_print_obj(void)
{
	obj_t *o=cons(sym("a"),cons(sym("b"),NULL));
	incr_refs(o);
	print_obj(o);
	putchar('\n');
	decr_refs(o);
}
void test_read(void)
{
	char buf[250];
	fgets(buf,250,stdin);
	obj_t *o=read(buf);
	incr_refs(o);
	fputs("Read: ",stdout);
	print_obj(o);
	putchar('\n');
	puts_type(o);
	decr_refs(o);
}
void test_table(void)
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
			print_obj(set(CAR(CDR(s)),CAR(CDR(CDR(s)))));
		} else if (!strcmp(CAR(s)->data.sym,"get")) {
			print_obj(get(CAR(CDR(s))));
		} else if (!strcmp(CAR(s)->data.sym,"unset")) {
			print_obj(unset(CAR(CDR(s))));
		}
		putchar('\n');
		decr_refs(s);
	}
	free_table(dict);
}
void test_rpn(void)
{
	char buf[250];
	fgets(buf,250,stdin);
	obj_t *input=read(buf);
	incr_refs(input);
	print_obj(input);
	putchar('\n');
	obj_t *translated=rpn(input);
	incr_refs(translated);
	print_obj(translated);
	putchar('\n');
	decr_refs(input);
	decr_refs(translated);
}
int main(int argc,char **argv)
{
	test_rpn();
}
