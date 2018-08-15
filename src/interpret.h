// Function for interpreting a list with stack-based postfix notation
#ifndef INTERPRET_H
#define INTERPRET_H
#include "function.h"
#include "namespace.h"
#include "object.h"
#include "stack.h"

#define ALL_ERRORS_FATAL
void interpret(obj_t *);
#endif
