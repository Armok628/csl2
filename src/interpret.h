// Function for interpreting a list with stack-based postfix notation
#ifndef INTERPRET_H
#define INTERPRET_H
#include "core.h"
#include "function.h"
#include "stack.h"
#include "object.h"

#define ALL_ERRORS_FATAL
void interpret(obj_t *);
#endif
