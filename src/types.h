#ifndef TYPES_H
#define TYPES_H
#include <stdio.h>
#include <stdlib.h>
typedef enum {false,true} bool;
typedef enum {
	CELL,
	DOUBLE,
	ERROR,
	INTEGER,
	FUNCTION,
	SYMBOL,
} type_t;
typedef struct obj_s {
	int refs;
	type_t type;
	union {
		struct {
			struct obj_s *car;
			struct obj_s *cdr;
		} cell;
		char *sym;
		long i;
		double d;
	} data;
} obj_t;
#define CAR(x) ((x)->data.cell.car)
#define CDR(x) ((x)->data.cell.cdr)

obj_t *new_obj();
obj_t *new_symbol(char *);
obj_t *new_integer(long);
obj_t *new_double(double);
void destroy(obj_t *);
void incr_refs(obj_t *);
void decr_refs(obj_t *);
obj_t *cons(obj_t *,obj_t *);
#endif
