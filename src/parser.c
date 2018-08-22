#include "parser.h"
bool valid_list(char *str)
{ // Matching number of parentheses
	int parens=0;
	for (;*str;str++) {
		parens+=*str=='(';
		parens-=*str==')';
	}
	return parens==0;
}
char *trim_left(char *str)
{ // Returns pointer to where printing characters begin
	for (;*str;str++) {
		switch (*str) {
		case '#':
			for (;*str&&*str!='\n';str++);
	 	case ' ':
		case '\n':
		case '\t':
			continue;
		default:
			return str;
		}
	}
	return str;
}
char *trim_right(char *str)
{ // Replaces whitespace at end of string with null
	int l=strlen(str)-1;
	for (;l>0;l--) {
		switch (str[l]) {
		case ' ':
		case '\n':
		case '\t':
			str[l]='\0';
		default:
			return str;
		}
	}
	return str; // Return string start
}
char *trim(char *s)
{
	return trim_left(trim_right(s));
}
type_t infer_type(char *s)
{ // Token argument checked as-is
	bool number=false;
	int periods=0;
	if (*s=='(') {
		if (valid_list(s))
			return CELL;
		return ERROR;
	}
	s+=*s=='-';
	for (;*s;s++) {
		if ('0'<=*s&&*s<='9') {
			number=true;
			continue;
		} else if (*s=='.') {
			periods++;
			if (periods>1)
				return SYMBOL;
		} else
			return SYMBOL;
	}
	if (!number)
		return SYMBOL;
	return periods?DOUBLE:INTEGER;
}
int delimit(char *str)
{ // Insert null characters between tokens. Returns number of tokens
	// Assumes valid list string starting with first parenthesis
	*str='\0';
	str++;
	int count=0;
	int parens=0;
	bool tok=false;
	for (;*str;str++) {
		switch (*str) {
		case '\\':
			tok=true;
			str++;
			break;
		case '(':
			parens++;
			break;
		case ')':
			parens--;
			if (parens<0) {
				*str='\0';
				count+=tok;
			}
			tok=true;
			break;
		case '\n':
		case '\t':
		case ' ':
			if (!parens) {
				*str='\0';
				count+=tok;
			}
			tok=false;
			break;
		default:
			tok=true;
		}
	}
	return count;
}
obj_t *read_list_str(char *str)
{ // Assumes valid list
	obj_t *ret=NULL;
	obj_t *tail=NULL;
	int n=delimit(str);
	char *tok=str;
	bool dotted=false;
	for (int i=0;i<n;i++) {
		for (;*tok;tok++); // Find next separator
		for (;!*tok;tok++) // Find next token
			*tok=' '; // Replace nulls with space to avoid double reading
		if (!strcmp(tok,".")) {
			dotted=true;
			continue;
		}
		obj_t *r=read_str(tok);
		r=dotted?r:new_cell(r,NULL);
		if (!ret) {
			ret=r;
			tail=ret;
		} else {
			CDR(tail)=incr_refs(r);
			tail=CDR(tail);
		}
	}
	return ret;
}
obj_t *quote(obj_t *obj)
{
	if (obj&&obj->type!=CELL&&obj->type!=SYMBOL)
		return obj;
	return new_cell(strsym("QUOTE"),new_cell(obj,NULL));
}
obj_t *read_splice_str(char *str)
{ // Accepts argument identical to read_list_str (no starting `)
	// `(A ,B @C . D) => (CONS (QUOTE A) (CONS B (NCONC C (QUOTE D))))
	obj_t *ret=NULL;
	obj_t *tail=NULL;
	int n=delimit(str);
	char *tok=str;
	bool dotted=false;
	for (int i=0;i<n;i++) {
		for (;*tok;tok++); // Find next separator
		for (;!*tok;tok++) // Find next token
			*tok=' '; // Replace nulls with space to avoid double reading
		if (!strcmp(tok,".")) {
			dotted=true;
			continue;
		}
		bool unquote=*tok==',';
		bool splice=*tok=='@';
		obj_t *r;
		if (*tok=='('&&!(unquote||splice))
			r=read_splice_str(tok);
		else {
			r=read_str(tok+unquote+splice);
			if (!(unquote||splice))
				r=quote(r);
		}
		if (splice&&i+1>=n)
			dotted=true;
		if (!dotted) {
			obj_t *s=strsym(splice?"APPEND":"CONS");
			r=new_cell(s,new_cell(r,new_cell(NULL,NULL)));
		}
		if (!ret) {
			ret=r;
			tail=CDR(CDR(ret));
		} else {
			CAR(tail)=incr_refs(r);
			if (dotted)
				break;
			tail=CDR(CDR(CAR(tail)));
		}
	}
	return ret;
}
obj_t *read_str(char *str)
{ // Error checking through infer_type
	obj_t *obj;
	str=trim(str); // Trims input
	if (!*str||!strcasecmp(str,"NIL"))
		return NULL;
	if (*str=='`')
		return read_splice_str(str+1);
	bool q=*str=='\'';
	str+=q;
	switch (infer_type(str)) {
	case CELL:
		obj=read_list_str(str);
		break;
	case SYMBOL:
		obj=strsym(str);
		break;
	case INTEGER:
		obj=new_integer(atol(str));
		break;
	case DOUBLE:
		obj=new_double(strtod(str,NULL));
		break;
	default:
		return strsym("ERROR");
	} // String must be freed by caller if necessary
	return q?quote(obj):obj;
}
obj_t *load_file(char *filename)
{
	FILE *fh=fopen(filename,"r");
	if (!fh)
		return NULL;
	fseek(fh,0,SEEK_END);
	long len=ftell(fh);
	fseek(fh,0,SEEK_SET);
	char *buf=calloc(len+1,1);
	fread(buf,1,len,fh);
	fclose(fh);
	obj_t *r=read_str(buf);
	free(buf);
	return r;
}
