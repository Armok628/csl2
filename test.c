#include <stdio.h>
#include <string.h>
#include "src/core.h"
#include "src/compile.h"
#include "src/interpret.h"
#include "src/namespace.h"
#include "src/parser.h"
#include "src/stack.h"
#include "src/types.h"
#include "init.h"
void puts_type(obj_t *o)
{
	if (!o)
		puts("NIL");
	else
		puts(type_names[o->type]);
}
void test_read(void)
{
	char buf[250];
	fgets(buf,250,stdin);
	obj_t *o=read(buf);
	incr_refs(o);
	print_obj(o);
	putchar('\n');
	puts_type(o);
	decr_refs(o);
}
void test_compile(void)
{
	char buf[250];
	fgets(buf,250,stdin);
	obj_t *input=read(buf);
	incr_refs(input);
	print_obj(input);
	putchar('\n');
	obj_t *translated=rpn(input);
	decr_refs(input);
	incr_refs(translated);
	print_obj(translated);
	putchar('\n');
	decr_refs(translated);
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
	interpret(t);
	decr_refs(t);
	fputs("=> ",stdout);
	stack_print();
	terpri();
	drop();
}
int main(int argc,char **argv)
{
	init_dict();
	test_compile_interpret();
	free_table(dict);
}
