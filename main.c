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
		push(strsym(argv[1]));
		stack_load();
		stack_eval();
		drop();
		goto QUIT_MAIN;
	}
#ifdef REPL
	for (;;)
#endif
		{
		char buf[1000];
		if (!fgets(buf,1000,stdin))
			goto QUIT_MAIN;
		push(read_str(buf));
		stack_eval();
		fputs("\n=> ",stdout);
		stack_print();
		drop();
		putchar('\n');
	}
QUIT_MAIN:
	free_table(dict);
	return 0;
}
