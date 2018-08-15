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
void bind_argv(int argc,char **argv)
{
	obj_t *l=NULL;
	for (int i=argc-1;i>=0;i--)
		l=new_cell(read_str(argv[i]),l);
	set_binding(new_symbol(strdup("ARGV")),l);
}
int main(int argc,char **argv)
{
	init_dict();
	if (argc>1) {
		bind_argv(argc,argv);
		char *script=slurp(argv[1]);
		if (!script) {
			printf("Could not read %s\n",argv[1]);
			return 1;
		}
		push(read_str(script));
		stack_eval();
		drop();
		free(script);
	} else {
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
	}
	free_table(dict);
	return 0;
}
