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
int main(int argc,char **argv)
{
	init_dict();
	if (argc>1) {
		char *script=slurp(argv[1]);
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
}
