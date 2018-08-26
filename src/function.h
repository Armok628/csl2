// Functions for creating LISP function objects
// and calling them in the LISP environment
#ifndef FUNCTION_H
#define FUNCTION_H
#include "interpret.h"
#include "object.h"

obj_t *new_cfunction(void (*)(void));
obj_t *new_function(obj_t *,obj_t *);
bool funcall(obj_t *);
#endif
