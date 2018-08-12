#include <stdio.h>
#include <string.h>
#include "src/core.h"
#include "src/compile.h"
#include "src/interpret.h"
#include "src/parser.h"
#include "src/stack.h"
#include "src/types.h"
#include "init.h"
obj_t *sym(const char *str)
{
	return new_symbol(strdup(str));
}
void puts_type(obj_t *o)
{
	if (!o)
		puts("NIL");
	else
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
		if (!strcasecmp(buf,"(quit)\n"))
			break;
		obj_t *s=read(buf);
		incr_refs(s);
		obj_t *r;
		if (!strcasecmp(CAR(s)->data.sym,"set")) {
			r=set(CAR(CDR(s)),CAR(CDR(CDR(s))));
			incr_refs(r);
			print_obj(r);
		} else if (!strcasecmp(CAR(s)->data.sym,"get")) {
			r=get(CAR(CDR(s)));
			incr_refs(r);
			print_obj(r);
		} else if (!strcasecmp(CAR(s)->data.sym,"unset")) {
			r=unset(CAR(CDR(s)));
			incr_refs(r);
			print_obj(r);
		}
		putchar('\n');
		decr_refs(r);
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
	fputs("Read: ",stdout);
	print_obj(input);
	putchar('\n');
	obj_t *translated=rpn(input);
	incr_refs(translated);
	print_obj(translated);
	putchar('\n');
	decr_refs(input);
	decr_refs(translated);
}
void test_stack(void)
{
	push(new_integer(1));
	push(new_integer(2));
	push(new_integer(3));
	push(NULL);
	stack_cons();
	stack_cons();
	stack_cons();
	stack_print();
	drop();
	stack_terpri();
	drop();
}
void test_interpret(void)
{
	char buf[250];
	fgets(buf,250,stdin);
	obj_t *r=read(buf);
	incr_refs(r);
	interpret(r);
	decr_refs(r);
	fputs("=> ",stdout);
	stack_print();
	terpri();
	drop();
}
void test_compile_interpret(void)
{
	char buf[250];
	fgets(buf,250,stdin);
	obj_t *r=incr_refs(read(buf));
	obj_t *t=incr_refs(rpn(r));
	decr_refs(r);
	fputs("Compiled: ",stdout);
	print_obj(t);
	terpri();
	interpret(t);
	decr_refs(t);
	fputs("=> ",stdout);
	stack_print();
	terpri();
	drop();
}
int main(int argc,char **argv)
{
	init();
	test_compile_interpret();
	free_table(dict);
}
