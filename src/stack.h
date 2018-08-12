#ifndef STACK_H
#define STACK_H
#include "types.h"
#define STACK_SIZE 1000
extern int stack_index;

void push(obj_t *);
obj_t *pop(void);
void drop(void);
#endif
