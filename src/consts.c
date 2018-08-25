#include "consts.h"
#define SYM_CONST(name,symbol) \
obj_t name={ \
	.type=SYMBOL, \
	.data={.sym=#symbol}, \
	.refs=1, \
};
SYM_CONST(append_sym,APPEND)
SYM_CONST(cond_sym,COND)
SYM_CONST(cond_end_sym,COND_END)
SYM_CONST(cons_sym,CONS)
SYM_CONST(drop_sym,DROP)
SYM_CONST(exec_sym,!)
SYM_CONST(list_sym,LIST)
SYM_CONST(list_end_sym,LIST_END)
SYM_CONST(quote_sym,QUOTE)
SYM_CONST(t_sym,T)
// Type Symbols
SYM_CONST(cell_sym,CELL)
SYM_CONST(double_sym,DOUBLE)
SYM_CONST(error_sym,ERROR)
SYM_CONST(function_sym,FUNCTION)
SYM_CONST(namespace_sym,NAMESPACE)
SYM_CONST(integer_sym,INTEGER)
SYM_CONST(symbol_sym,SYMBOL)
