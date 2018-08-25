// Functions for creating new LISP objects
#ifndef NEW_H
#define NEW_H
#include <stdlib.h>
#include "hash.h"
#include "object.h"
#define CONS new_cell
obj_t *new_object(void);
obj_t *new_cell(obj_t *,obj_t *);
obj_t *new_symbol(char *);
obj_t *new_integer(long);
obj_t *new_double(double);
obj_t *new_namespace_obj(table_t *);
obj_t *new_cfunction(void (*)(void));
#endif
