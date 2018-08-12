// Function for parsing source code into LISP objects
#ifndef PARSE_H
#define PARSE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "object.h"

obj_t *read_str(char *);
#endif
