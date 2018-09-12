// Functions for manipulating the stack used for interpretation
#ifndef STACK_H
#define STACK_H
#include "object.h"
#define STACK_SIZE 1000
extern obj_t *stack[STACK_SIZE];
extern int tos;

void push(obj_t *);
obj_t *pop(void);
obj_t *dpop(void);
void drop(void);
int print_stack(void);
#endif
