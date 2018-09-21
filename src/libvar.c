#include "libvar.h"
#ifdef LIBRARY_FILE
obj_t libvar={
	.type=SYMBOL,
	.data={.sym=LIBRARY_FILE},
	.refs=1,
};
#endif
