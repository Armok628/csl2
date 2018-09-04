#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "src/compile.h"
#include "src/interpret.h"
#include "src/namespace.h"
#include "src/object.h"
#include "src/parser.h"
#include "src/stack.h"
void bind_argv(int argc,char **argv)
{
	obj_t *l=NULL;
	for (int i=argc-1;i>=0;i--)
		l=new_cell(read_str(argv[i]),l);
	insert(dict,"ARGV",incr_refs(l));
}
int main(int argc,char **argv)
{
	init_dict();
	obtable=new_table(NAMESPACE_SIZE,NULL);
	intern_consts();
	if (argc>1) {
		bind_argv(argc,argv);
		obj_t *r=incr_refs(load_file(argv[1]));
		obj_t *t=incr_refs(rpn(r));
		decr_refs(r);
		interpret(t);
		decr_refs(t);
		drop();
		goto QUIT_MAIN;
	}
	for (;;) {
		char buf[1000];
		if (!fgets(buf,1000,stdin))
			goto QUIT_MAIN;
		obj_t *r=incr_refs(read_str(buf));
		obj_t *t=incr_refs(rpn(r));
		decr_refs(r);
		interpret(t);
		decr_refs(t);
		r=pop();
		fputs("\n=> ",stdout);
		print_obj(r,stdout);
		decr_refs(r);
		putchar('\n');
	}
QUIT_MAIN:
	unset_binding("DICTIONARY");
	free_table(obtable);
	return 0;
}
