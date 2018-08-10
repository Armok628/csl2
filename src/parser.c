#include "parser.h"
char *strclone(char *str)
{
	int n=strlen(str);
	return strncpy(calloc(n+1,1),str,n);
}
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
obj_t *readlist(char *str)
{ // Assumes valid list
	obj_t *ret=NULL;
	obj_t **tail=NULL;
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
		obj_t *r=read(tok);
		if (!ret) {
			ret=dotted?r:cons(r,NULL);
			tail=&CDR(ret);
		} else {
			*tail=dotted?r:cons(r,NULL);
			tail=&CDR(*tail);
		}
	}
	return ret;
}
obj_t *read(char *str)
{ // Error checking through infer_type
	str=trim(str); // Trims input
	switch (infer_type(str)) {
	case CELL:
		return readlist(str);
	case SYMBOL:
		return new_symbol(strdup(str));
	case INTEGER:
		return new_integer(atol(str));
	case DOUBLE:
		return new_double(strtod(str,NULL));
	default:
		break;
	} // String must be freed by caller if necessary
	return new_symbol(strdup("ERROR"));
}
