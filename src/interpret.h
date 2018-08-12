#ifndef INTERPRET_H
#define INTERPRET_H
#include "core.h"
#include "stack.h"
#include "types.h"

void interpret(obj_t *);
void funcall(obj_t *);
#endif
