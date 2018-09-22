#include "libvar.h"
#ifdef LIBFILE
obj_t libvar={
	.type=SYMBOL,
	.data={.sym=LIBFILE},
	.refs=1,
};
#endif
