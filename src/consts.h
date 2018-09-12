// Static objects defined to avoid
// dynamically allocating common objects
#ifndef CONSTS_H
#define CONSTS_H
#include "object.h"

extern obj_t *error;
extern obj_t append_sym;
extern obj_t cond_sym;
extern obj_t cond_end_sym;
extern obj_t cons_sym;
extern obj_t drop_sym;
extern obj_t execute_sym;
extern obj_t if_sym;
extern obj_t list_sym;
extern obj_t list_end_sym;
extern obj_t print_sym;
extern obj_t print_end_sym;
extern obj_t progn_sym;
extern obj_t quote_sym;
extern obj_t t_sym;
#define T &t_sym
// Normally-escaped symbols
extern obj_t newline_sym;
extern obj_t tab_sym;
// Type Symbols
extern obj_t array_sym;
extern obj_t cell_sym;
extern obj_t double_sym;
extern obj_t error_sym;
extern obj_t function_sym;
extern obj_t namespace_sym;
extern obj_t integer_sym;
extern obj_t symbol_sym;

void intern_consts(void);
#endif
