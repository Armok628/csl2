#include "stack.h"
static obj_t *stack[STACK_SIZE];
static int stack_index=0;
void push(obj_t *obj)
{
	if (stack_index>=STACK_SIZE) {
		fputs("Stack overflow\n",stderr);
		exit(1);
	}
	stack[stack_index++]=incr_refs(obj);
}
obj_t *pop(void)
{ // Popped stack items must have a call to decr_refs after use
	if (stack_index<=0) {
		fputs("Stack underflow\n",stderr);
		exit(1);
	}
	return stack[--stack_index];
}
obj_t *dpop(void)
{ // Decreases reference counter without destroying
	obj_t *r=stack[--stack_index];
	if (r)
		r->refs--;
	return r;
}
void drop(void)
{
	decr_refs(stack[--stack_index]);
}
obj_t *stack_obj(int n)
{
	return stack[stack_index-n-1];
}
int print_stack(void)
{ // Returns number of stack items
	if (!stack_index) {
		fputs("Empty stack\n",stderr);
		return 0;
	}
	fprintf(stderr,"Stack (%d):\n",stack_index);
	for (int i=stack_index-1;i>=0;i--) {
		fprintf(stderr,"%d: ",i);
		print_obj(stack[i],stderr);
		fputc('\n',stderr);
	}
	return stack_index;
}
