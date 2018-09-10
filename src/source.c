#include "source.h"
// ^ Include this file in namespace.h
obj_t *source(obj_t *filename)
{ // ^ Return types and arguments must be of type obj_t *.
	if (!type_check(filename,SYMBOL,"SOURCE: "))
		// ^ Prints an error message if argument is of bad type
		return new_object();
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
void init_source(void)
{ // ^ Call this function from init_dict() in namespace.c
	INIT(SOURCE,source);
	// ^ Bind the SOURCE variable to the stack-based function
}
