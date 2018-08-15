// Type definitions, functions, and macros
// for creating and manipulating LISP objects
#ifndef TYPES_H
#define TYPES_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
typedef enum {false,true} bool;
typedef enum {
	CELL=1<<0,
	DOUBLE=1<<1,
	ERROR=1<<2,
	FUNCTION=1<<3,
	HASHTABLE=1<<4,
	INTEGER=1<<5,
	NIL=1<<6, // Pseudo-type for use in type_check
	SYMBOL=1<<7,
} type_t; // Formatted as flags for type_check
typedef struct obj_s {
	int refs;
	type_t type;
	union {
		struct {
			struct obj_s *car;
			struct obj_s *cdr;
		} cell;
		struct {
			bool lambda;
			union {
				void (*c)(void);
				struct obj_s *lisp; // (namespace_obj args_list body)
			} rep;
		} func;
		char *sym;
		long i;
		double d;
		table_t *table;
	} data;
} obj_t;
#include "new.h"
#define CAR(x) ((x)->data.cell.car)
#define CDR(x) ((x)->data.cell.cdr)

const char *type_name(type_t);
const char *obj_type_name(obj_t *);
bool type_check(obj_t *,type_t,const char *);
void destroy(obj_t *);
obj_t *incr_refs(obj_t *);
void decr_refs(obj_t *);
void print_cell(obj_t *);
void print_obj(obj_t *);
bool eq_objs(obj_t *,obj_t *);
bool symbol_match(obj_t *,const char *);
int list_length(obj_t *);
obj_t *copy_obj(obj_t *);
void concatenate(obj_t *,obj_t *);
#endif
