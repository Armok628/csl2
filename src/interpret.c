#include "interpret.h"
obj_t *interpret_cond(obj_t *l)
{ // Returns list at COND_END
	l=CDR(l);
	bool ret=false;
	for (;!ret&&l&&CAR(l)!=&cond_end_sym;l=CDR(CDR(l))) {
		interpret(CAR(l));
		obj_t *r=pop();
		if (r) {
			interpret(CAR(CDR(l)));
			ret=true;
		}
		decr_refs(r);
	}
	if (!ret)
		push(NULL);
	while (l&&CAR(l)!=&cond_end_sym)
		l=CDR(l);
	return l;
}
void interpret(obj_t *list)
{ // Requires RPN list as argument
	obj_t *fs=NULL;
	for (obj_t *o=list;o;o=CDR(o)) {
		obj_t *instr=CAR(o);
		if (!instr) {
			push(NULL);
		} else if (instr->type!=SYMBOL) {
			push(instr);
		} else if (instr==&list_sym) {
			push(instr);
		} else if (instr==&list_end_sym) {
			obj_t *body=NULL;
			while (stack[tos]!=&list_sym)
				body=new_cell(dpop(),body);
			drop();
			push(body);
		} else if (instr==&drop_sym) {
			drop();
		} else if (instr==&execute_sym) {
			obj_t *f=pop();
			if (!funcall(f))
				goto FATAL_INTERP_ERROR;
			decr_refs(f);
#ifdef ALL_ERRORS_FATAL
			obj_t *r=stack[tos];
			if (r&&r->type==ERROR) {
				drop();
				goto FATAL_INTERP_ERROR;
			}
#endif
		} else if (instr==&print_sym) {
			obj_t *o=pop();
			if (o&&o->type==FILESTREAM&&o->data.fs)
				fs=o;
			else {
				print_obj(o,fs?fs->data.fs:stdout);
				decr_refs(o);
			}
		} else if (instr==&print_end_sym) {
			decr_refs(fs);
			fs=NULL;
		} else if (instr==&quote_sym) {
			o=CDR(o);
			push(CAR(o));
		} else if (instr==&cond_sym) {
			o=interpret_cond(o);
			// COND_END skipped by o=CDR(o) in for-loop
		} else {
			push(get_binding(instr->data.sym));
		}
	}
	return;

FATAL_INTERP_ERROR:
	fputs("Fatal error while interpreting ",stderr);
	print_obj(list,stderr);
	fputc('\n',stderr);
	print_stack();
	exit(1);
}

