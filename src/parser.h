#ifndef PARSE_H
#define PARSE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

char *strclone(char *);
bool valid_list(char *);
char *trim_left(char *);
char *trim_right(char *);
char *trim(char *);
type_t infer_type(char *);
int nullspace(char *);
obj_t *readlist(char *);
obj_t *read(char *);
#endif
