#include "consts.h"
// This file is structured similarly to core.c
// Constant symbols are interned the same way functions are bound
#define SYM_CONST(name,symbol) \
obj_t name={ \
	.type=SYMBOL, \
	.data={.sym=symbol}, \
	.refs=1, \
};
SYM_CONST(append_sym,"APPEND")
SYM_CONST(cond_sym,"COND")
SYM_CONST(cond_end_sym,"COND_END")
SYM_CONST(cons_sym,"CONS")
SYM_CONST(drop_sym,"DROP")
SYM_CONST(execute_sym,"EXECUTE")
SYM_CONST(if_sym,"IF")
SYM_CONST(list_sym,"LIST")
SYM_CONST(list_end_sym,"LIST_END")
SYM_CONST(print_sym,"PRINT")
SYM_CONST(progn_sym,"PROGN")
SYM_CONST(quote_sym,"QUOTE")
SYM_CONST(t_sym,"T")
SYM_CONST(terpri_sym,"\n")
// Type Symbols
SYM_CONST(array_sym,"ARRAY")
SYM_CONST(cell_sym,"CELL")
SYM_CONST(double_sym,"DOUBLE")
SYM_CONST(error_sym,"ERROR")
SYM_CONST(function_sym,"FUNCTION")
SYM_CONST(namespace_sym,"NAMESPACE")
SYM_CONST(integer_sym,"INTEGER")
SYM_CONST(symbol_sym,"SYMBOL")
#define INTERN_CONST(constsym) \
insert(obtable,constsym.data.sym,&constsym);
void intern_consts(void)
{
	INTERN_CONST(append_sym)
	INTERN_CONST(cond_sym)
	INTERN_CONST(cond_end_sym)
	INTERN_CONST(cons_sym)
	INTERN_CONST(drop_sym)
	INTERN_CONST(execute_sym)
	INTERN_CONST(if_sym)
	INTERN_CONST(list_sym)
	INTERN_CONST(list_end_sym)
	INTERN_CONST(print_sym)
	INTERN_CONST(progn_sym)
	INTERN_CONST(quote_sym)
	INTERN_CONST(t_sym)
	// Type Symbols
	INTERN_CONST(array_sym)
	INTERN_CONST(cell_sym)
	INTERN_CONST(double_sym)
	INTERN_CONST(error_sym)
	INTERN_CONST(function_sym)
	INTERN_CONST(namespace_sym)
	INTERN_CONST(integer_sym)
	INTERN_CONST(symbol_sym)
}
