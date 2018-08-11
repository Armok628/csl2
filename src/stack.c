#include "stack.h"
static obj_t *stack[STACK_SIZE];
static int next=0;
void push(obj_t *obj)
{
	incr_refs(obj);
	stack[next++]=obj;
}
obj_t *pop(void)
{ // Popped stack items must have a call to decr_refs after use
	return stack[--next];
}
void drop(void)
{
	decr_refs(stack[--next]);
}
