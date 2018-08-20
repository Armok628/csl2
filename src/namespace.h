// Functions for creating and manipulating LISP namespaces
#ifndef NAMESPACE_H
#define NAMESPACE_H
#include "object.h"
#include "stack.h"
// The following headers contain init functions for built-ins
#include "arith.h"
#include "core.h"
#define NAMESPACE_SIZE 100
extern table_t *dict;
extern int level;

void init_dict(void);
void push_namespace(table_t *);
table_t *pop_namespace(void);
void drop_namespace(void);
table_t *new_namespace_table(int);
obj_t *get_binding(char *);
void set_binding(char *,obj_t *);
void unset_binding(char *);
#endif
