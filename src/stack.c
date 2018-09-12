#include "stack.h"
obj_t *stack[STACK_SIZE];
int tos=-1; // "Top of stack" -- Last filled slot
void push(obj_t *obj)
{
	if (tos>=STACK_SIZE) {
		fputs("Stack overflow\n",stderr);
		exit(1);
	}
	stack[++tos]=incr_refs(obj);
}
obj_t *pop(void)
{ // Popped stack items must have a call to decr_refs after use
	if (tos<0) {
		fputs("Stack underflow\n",stderr);
		exit(1);
	}
	return stack[tos--];
}
obj_t *dpop(void)
{ // Decreases reference counter without destroying
	obj_t *r=stack[tos--];
	if (r)
		r->refs--;
	return r;
}
void drop(void)
{
	decr_refs(stack[tos--]);
}
int print_stack(void)
{ // Returns number of stack items
	if (tos==-1) {
		fputs("Empty stack\n",stderr);
		return 0;
	}
	fprintf(stderr,"Stack (%d):\n",tos+1);
	for (int i=tos;i>=0;i--) {
		fprintf(stderr,"%d: ",i);
		print_obj(stack[i],stderr);
		fputc('\n',stderr);
	}
	return tos+1;
}
