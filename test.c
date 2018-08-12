#include <stdio.h>
#include <string.h>
#include "src/core.h"
#include "src/compile.h"
#include "src/interpret.h"
#include "src/namespace.h"
#include "src/parser.h"
#include "src/stack.h"
#include "src/object.h"
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
		push(new_integer(1000));
		stack_read();
		stack_eval();
		putchar('\n');
		fputs("=> ",stdout);
		stack_print();
		drop();
		putchar('\n');
	}
	free_table(dict);
}
