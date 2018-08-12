// Functions for creating LISP function objects
// and calling them in the LISP environment
#ifndef FUNCTION_H
#define FUNCTION_H
#include "interpret.h"
#include "types.h"

obj_t *new_cfunction(void (*)(void));
obj_t *new_lispfunction(obj_t *,obj_t *);
void funcall(obj_t *);
#endif
