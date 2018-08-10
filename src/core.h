#ifndef CORE_H
#define CORE_H
#include "types.h"
#include "hash.h"

extern table_t *dict;
obj_t *cons(obj_t *,obj_t *);
obj_t *car(obj_t *);
obj_t *cdr(obj_t *);
obj_t *rplaca(obj_t *,obj_t *);
obj_t *rplacd(obj_t *,obj_t *);
obj_t *set(obj_t *,obj_t *);
obj_t *get(obj_t *);
obj_t *unset(obj_t *);
#endif
