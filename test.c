#include <stdio.h>
#include <string.h>
#include "src/core.h"
#include "src/compile.h"
#include "src/interpret.h"
#include "src/namespace.h"
#include "src/parser.h"
#include "src/stack.h"
#include "src/object.h"
void test_read(void)
{
	char buf[250];
	fgets(buf,250,stdin);
	obj_t *o=read(buf);
	incr_refs(o);
	print_obj(o);
	putchar('\n');
	puts(obj_type_name(o));
	decr_refs(o);
}
void test_compile(void)
{
	char buf[250];
	fgets(buf,250,stdin);
	obj_t *input=incr_refs(read(buf));
	print_obj(input);
	putchar('\n');
	obj_t *translated=incr_refs(rpn(input));
	decr_refs(input);
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
void test_eval(void)
{
	char buf[250];
	fgets(buf,250,stdin);
	push(read(buf));
	stack_eval();
	fputs("=> ",stdout);
	stack_print();
	drop();
	terpri();
	drop();
}
char *slurp(char *filename)
{
	FILE *fh=fopen(filename,"r");
	if (!fh)
		return NULL;
	fseek(fh,0,SEEK_END);
	long len=ftell(fh);
	fseek(fh,0,SEEK_SET);
	char *buf=calloc(len+1,1);
	fread(buf,1,len,fh);
	fclose(fh);
	return buf;
}
int main(int argc,char **argv)
{
	init_dict();
	if (argc>1) {
		char *script=slurp(argv[1]);
		push(read(script));
		stack_eval();
		drop();
	} else
		test_eval();
	free_table(dict);
}
