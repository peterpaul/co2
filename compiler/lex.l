%{
#include "grammar.tab.h"
#include "Token.h"
#include "RefList.h"
#include "IncludeStack.h"

int linenumber = 1;
const char * filename;

static const char * input_file_name = NULL;
struct RefList * path = NULL;

void new_input(const char *);
%}

DIGIT		[0-9]
LETTER		[a-zA-Z]
UPPER_LETTER	[A-Z]
LOWER_LETTER	[a-z]
USCORE		_

INT_CONST	{DIGIT}+
FLOAT_CONST	{DIGIT}*\.{DIGIT}+(e-?{DIGIT}+)?
IDENTIFIER	{LOWER_LETTER}({LETTER}|{USCORE}|{DIGIT})*
TYPE_IDENT	{UPPER_LETTER}({LETTER}|{USCORE}|{DIGIT})*
MACRO_IDENT	{USCORE}({UPPER_LETTER}|{USCORE}|{DIGIT})*
NON_ESCAPED	[^\\\n\'\"]
ESCAPED		[ntvbrfa\\\?\'\"]|[0-7]+|x[0-9A-Fa-f]+
CHAR_SYMBOL	{NON_ESCAPED}|\\{ESCAPED}
CHAR_CONST	\'({CHAR_SYMBOL}|\")\'
CHAR_CONST_ERR	\'({CHAR_SYMBOL}|\")*\'
STRING_CONST	\"({CHAR_SYMBOL}|\')*\"

%%
"char"		yylval.token = O_CALL_CLASS(Token(), new, yytext, CHAR, filename, linenumber); return yylval.token->type;
"class"		return CLASS;
"delete"	return DELETE;
"do"		return DO;
"else"		return ELSE;
"float"		yylval.token = O_CALL_CLASS(Token(), new, yytext, FLOAT, filename, linenumber); return yylval.token->type;
"for"		return FOR;
"if"		return IF;
"import"	return IMPORT;
"int"		yylval.token = O_CALL_CLASS(Token(), new, yytext, INT, filename, linenumber); return yylval.token->type;
"new"		return NEW;
"package"	return PACKAGE;
"return"	return RETURN;
"self"		yylval.token = O_CALL_CLASS(Token(), new, yytext, SELF, filename, linenumber); return yylval.token->type;
"super"		yylval.token = O_CALL_CLASS(Token(), new, yytext, SUPER, filename, linenumber); return yylval.token->type;
"unsigned"	return UNSIGNED;
"void"		yylval.token = O_CALL_CLASS(Token(), new, yytext, VOID, filename, linenumber); return yylval.token->type;
"while"		return WHILE;
"..."		return VA_ARG;
"&&"		yylval.token = O_CALL_CLASS(Token(), new, yytext, AND, filename, linenumber); return yylval.token->type;
"||"		yylval.token = O_CALL_CLASS(Token(), new, yytext, OR, filename, linenumber); return yylval.token->type;
"=="		yylval.token = O_CALL_CLASS(Token(), new, yytext, EQ, filename, linenumber); return yylval.token->type;
"!="		yylval.token = O_CALL_CLASS(Token(), new, yytext, NEQ, filename, linenumber); return yylval.token->type;
"##"		yylval.token = O_CALL_CLASS(Token(), new, yytext, XOR, filename, linenumber); return yylval.token->type;
"<="		yylval.token = O_CALL_CLASS(Token(), new, yytext, LEQ, filename, linenumber); return yylval.token->type;
">="		yylval.token = O_CALL_CLASS(Token(), new, yytext, GEQ, filename, linenumber); return yylval.token->type;
">>"		yylval.token = O_CALL_CLASS(Token(), new, yytext, SHIFTR, filename, linenumber); return yylval.token->type;
"<<"		yylval.token = O_CALL_CLASS(Token(), new, yytext, SHIFTL, filename, linenumber); return yylval.token->type;
{INT_CONST}	yylval.token = O_CALL_CLASS(Token(), new, yytext, INT_CONSTANT, filename, linenumber); return yylval.token->type;
{FLOAT_CONST}	yylval.token = O_CALL_CLASS(Token(), new, yytext, FLOAT_CONSTANT, filename, linenumber); return yylval.token->type;
{IDENTIFIER}	yylval.token = O_CALL_CLASS(Token(), new, yytext, IDENTIFIER, filename, linenumber); return yylval.token->type;
{TYPE_IDENT}	yylval.token = O_CALL_CLASS(Token(), new, yytext, TYPE_IDENTIFIER, filename, linenumber); return yylval.token->type;
{MACRO_IDENT}	yylval.token = O_CALL_CLASS(Token(), new, yytext, MACRO_IDENTIFIER, filename, linenumber); return yylval.token->type;
{CHAR_CONST}	yylval.token = O_CALL_CLASS(Token(), new, yytext, CHAR_CONSTANT, filename, linenumber); return yylval.token->type;
{CHAR_CONST_ERR}  yylval.token = O_CALL_CLASS(Token(), new, yytext, CHAR_CONSTANT, filename, linenumber); error(yylval.token, "invalid character constant %s", yytext); return yylval.token->type;
{STRING_CONST}	yylval.token = O_CALL_CLASS(Token(), new, yytext, STRING_CONSTANT, filename, linenumber); return yylval.token->type;

\n		linenumber++; /* count lines */
[ \t]		/* eat whitespace */
[\(\)\]\{\}\;\:\,]	return yytext[0]; /* no Token for these characters */
.		yylval.token = O_CALL_CLASS(Token(), new, yytext, yytext[0], filename, linenumber); return yylval.token->type;
%%
/*
int yywrap ()
{
  return 1;
}
*/
struct RefList *include_stack = NULL;

int yywrap()
{
	yy_delete_buffer(YY_CURRENT_BUFFER);
	fclose(yyin);

	if (!include_stack || include_stack->length == 0) {
		O_BRANCH_CALL(include_stack, delete);
		include_stack = NULL;
		yytext = "";
		return 1;
	}
	/* The input_file_name is only allocated for include files, and not for
	   the original input file. Therefore this call to free is after the
	   check on the include level. */
	free((char *) input_file_name);
	struct IncludeStack * item = O_CALL(include_stack, remove);
	yy_switch_to_buffer(item->buffer_state);
	linenumber = item->current_line;
	input_file_name = item->current_file;
	O_CALL(item, delete);
	return 0;
}

static void try_search_path(const char * file_name)
{
	unsigned i;
	
	for (i = 0; i < path->length; i++) {
		struct String * item = (struct String *)O_CALL(path, get, i);
		struct String * full_path = O_CALL_CLASS(String(), new, "%s", item->data);
		O_CALL(full_path, append_str, "%s", file_name);
		if ((yyin = fopen(full_path->data, "r")) != 0) {
			fprintf(stderr, "Found file: %s\n", full_path->data);
			O_CALL(full_path, delete);
			return;
		}
		O_CALL(full_path, delete);
	}

	error(NULL, "cannot open %s", file_name);
	exit(EXIT_FAILURE);
}

// char * strdup(const char *);
void new_input(const char * file_name)
{
	if (!include_stack) {
		include_stack = O_CALL_CLASS(RefList(), new, 8, IncludeStack());
	}
	O_CALL(include_stack, append, 
		O_CALL_CLASS(IncludeStack(), new, linenumber, input_file_name, YY_CURRENT_BUFFER));
	try_search_path(file_name);
	input_file_name = strdup(file_name);
	linenumber = 1;
	yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
	BEGIN(INITIAL);
}
