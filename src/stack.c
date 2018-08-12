#include "stack.h"
static obj_t *stack[STACK_SIZE];
int stack_index=0;
void push(obj_t *obj)
{
	incr_refs(obj);
	stack[stack_index++]=obj;
}
obj_t *pop(void)
{ // Popped stack items must have a call to decr_refs after use
	return stack[--stack_index];
}
void drop(void)
{
	decr_refs(stack[--stack_index]);
}
