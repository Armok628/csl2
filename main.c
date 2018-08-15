#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "src/core.h"
#include "src/compile.h"
#include "src/interpret.h"
#include "src/namespace.h"
#include "src/parser.h"
#include "src/stack.h"
#include "src/object.h"
#define REPL
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
	if (argc>1) {
		bind_argv(argc,argv);
		push(new_symbol(strdup(argv[1])));
		stack_load();
		stack_eval();
		drop();
	} else
#ifdef REPL
		for (;;)
#endif
		{
		push(new_integer(1000));
		stack_lread();
		stack_eval();
		fputs("\n=> ",stdout);
		stack_print();
		drop();
		putchar('\n');
	}
	free_table(dict);
	return 0;
}
