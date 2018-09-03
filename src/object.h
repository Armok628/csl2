// Type definitions, functions, and macros
// for creating and manipulating LISP objects
#ifndef TYPES_H
#define TYPES_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hash.h"
typedef enum {
	CELL=1<<0,
	DOUBLE=1<<1,
	ERROR=1<<2,
	FUNCTION=1<<3,
	NAMESPACE=1<<4,
	INTEGER=1<<5,
	NIL=1<<6, // Pseudo-type for use in type_check
	SYMBOL=1<<7,
} type_t; // Formatted as bit flags for type_check
typedef struct object {
	int refs;
	type_t type;
	union {
		struct {
			struct object *car,*cdr;
		} cell;
		struct {
			bool lambda;
			union {
				void (*c)(void);
				struct object *lisp;
			} rep;
		} func;
		char *sym;
		long i;
		double d;
		table_t *table;
	} data;
} obj_t; // (obj_t *)NULL represents NIL
#include "new.h"
#define CAR(x) ((x)->data.cell.car)
#define CDR(x) ((x)->data.cell.cdr)
#define LIST3(a,b,c) new_cell(a,LIST2(b,c))
#define LIST2(a,b) new_cell(a,LIST1(b))
#define LIST1(a) new_cell(a,NULL)

char *obj_type_name(obj_t *);
bool type_check(obj_t *,type_t,const char *);
obj_t *incr_refs(obj_t *);
void decr_refs(obj_t *);
void print_obj(obj_t *,FILE *);
bool eq_objs(obj_t *,obj_t *);
int list_length(obj_t *);
obj_t *copy_cell(obj_t *);
void concatenate(obj_t *,obj_t *);
#endif
