#include "example.h"
// ^ Include this file in namespace.h
obj_t *source(obj_t *filename)
{ // ^ Return types and arguments must be of type obj_t *.
	if (!type_check(filename,SYMBOL,"SOURCE: "))
		// ^ Prints an error message if argument is of bad type
		return error;
		// ^ Returns an uninitialized object with type ERROR
	obj_t *r=incr_refs(load_file(filename->data.sym));
	// ^ Increase references because it is generated for temporary use
	obj_t *t=incr_refs(rpn(r));
	// ^ Increase references because it is generated for temporary use
	decr_refs(r);
	// ^ Decrease references because we no longer need it
	interpret(t);
	decr_refs(t);
	// ^ Decrease references because we no longer need it
	return dpop();
	// ^ Call dpop() instead of pop() to decrement refs without destroying
}
STACK(source,1)
// ^ Generate a stack-based function with 1 argument that uses the function above
obj_t *incr(obj_t *var)
{
	if (!type_check(var,SYMBOL,"INCR, variable: "))
		return error;
	obj_t *o=incr_refs(get_binding(var->data.sym));
	if (!type_check(o,DOUBLE|INTEGER,"INCR, value: "))
		return o;
	obj_t *n;
	switch (o->type) {
	case DOUBLE:
		n=new_double(o->data.d+1);
		break;
	case INTEGER:
		n=new_integer(o->data.i+1);
		break;
	default: // Silence warnings about unchecked cases
		n=NULL;
		break;
	}
	decr_refs(o);
	set_binding(var->data.sym,n);
	return n;
}
STACK(incr,1)
void init_example(void)
{ // ^ Call this function from init_dict() in namespace.c
	INIT(SOURCE,source);
	INIT(++,incr);
	// ^ Bind the SOURCE variable to the stack-based function
}
