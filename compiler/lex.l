%{
#include "grammar.tab.h"
#include "Token.h"
#include "RefList.h"
#include "IncludeStack.h"
#include "String.h"
#include "List.h"
#include <limits.h>
#include <stdlib.h>

const char * filename;

struct RefList * path;

void new_input(struct String *);
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



%x MULTI_LINE_COMMENT
%x ONE_LINE_COMMENT
%x INCLUDE
%%
<INITIAL>{
	"/*"		BEGIN(MULTI_LINE_COMMENT);
	"//"		BEGIN(ONE_LINE_COMMENT);
	"include"	BEGIN(INCLUDE);
}
<MULTI_LINE_COMMENT>{
	"*/"		BEGIN(INITIAL);
	[^*\n]+		// eat comment in chunks
	"*"		// eat the lone star
	\n		linenumber++;
}
<ONE_LINE_COMMENT>{
	[^\n]+		// eat comment in chunks
	\n		linenumber++; BEGIN(INITIAL);
	<<EOF>>		BEGIN(INITIAL);
}
<INCLUDE>[ \t]*		/* eat the whitespace */
<INCLUDE>[^ \t\n]+	{ new_input (O_CALL_CLASS (String (), new, "%s", yytext)); }
"catch"		return CATCH;
"char"		yylval.token = O_CALL_CLASS(Token(), new, yytext, CHAR, filename, linenumber); return yylval.token->type;
"class"		return CLASS;
"delete"	return DELETE;
"do"		return DO;
"else"		return ELSE;
"finally"	return FINALLY;
"float"		yylval.token = O_CALL_CLASS(Token(), new, yytext, FLOAT, filename, linenumber); return yylval.token->type;
"for"		return FOR;
"if"		return IF;
"import"	return IMPORT;
"int"		yylval.token = O_CALL_CLASS(Token(), new, yytext, INT, filename, linenumber); return yylval.token->type;
"interface"	return INTERFACE;
"new"		return NEW;
"null"		yylval.token = O_CALL_CLASS(Token(), new, yytext, _NULL, filename, linenumber); return yylval.token->type;
"package"	return PACKAGE;
"return"	return RETURN;
"self"		yylval.token = O_CALL_CLASS(Token(), new, yytext, SELF, filename, linenumber); return yylval.token->type;
"sizeof"	return SIZEOF;
"super"		yylval.token = O_CALL_CLASS(Token(), new, yytext, SUPER, filename, linenumber); return yylval.token->type;
"throw"		return THROW;
"try"		return TRY;
"unsigned"	yylval.token = O_CALL_CLASS(Token(), new, yytext, UNSIGNED, filename, linenumber); return yylval.token->type;
"va_arg"	return GET_VA_ARG;
"va_list"	yylval.token = O_CALL_CLASS(Token(), new, yytext, VA_LIST, filename, linenumber); return yylval.token->type;
"void"		yylval.token = O_CALL_CLASS(Token(), new, yytext, VOID, filename, linenumber); return yylval.token->type;
"while"		return WHILE;
"..."		yylval.token = O_CALL_CLASS(Token(), new, yytext, VA_ARG, filename, linenumber); return yylval.token->type;
"&&"		yylval.token = O_CALL_CLASS(Token(), new, yytext, AND, filename, linenumber); return yylval.token->type;
"||"		yylval.token = O_CALL_CLASS(Token(), new, yytext, OR, filename, linenumber); return yylval.token->type;
"=="		yylval.token = O_CALL_CLASS(Token(), new, yytext, EQ, filename, linenumber); return yylval.token->type;
"!="		yylval.token = O_CALL_CLASS(Token(), new, yytext, NEQ, filename, linenumber); return yylval.token->type;
"##"		yylval.token = O_CALL_CLASS(Token(), new, yytext, XOR, filename, linenumber); return yylval.token->type;
"<="		yylval.token = O_CALL_CLASS(Token(), new, yytext, LEQ, filename, linenumber); return yylval.token->type;
">="		yylval.token = O_CALL_CLASS(Token(), new, yytext, GEQ, filename, linenumber); return yylval.token->type;
">>"		yylval.token = O_CALL_CLASS(Token(), new, yytext, SHIFTR, filename, linenumber); return yylval.token->type;
"<<"		yylval.token = O_CALL_CLASS(Token(), new, yytext, SHIFTL, filename, linenumber); return yylval.token->type;
"+="		yylval.token = O_CALL_CLASS(Token(), new, yytext, INCREASE, filename, linenumber); return yylval.token->type;
"-="		yylval.token = O_CALL_CLASS(Token(), new, yytext, DECREASE, filename, linenumber); return yylval.token->type;
"*="		yylval.token = O_CALL_CLASS(Token(), new, yytext, MULTIPLY, filename, linenumber); return yylval.token->type;
"/="		yylval.token = O_CALL_CLASS(Token(), new, yytext, DIVIDE, filename, linenumber); return yylval.token->type;
"^="		yylval.token = O_CALL_CLASS(Token(), new, yytext, POWER, filename, linenumber); return yylval.token->type;
"%="		yylval.token = O_CALL_CLASS(Token(), new, yytext, REMINDER, filename, linenumber); return yylval.token->type;
"&="		yylval.token = O_CALL_CLASS(Token(), new, yytext, AND_IS, filename, linenumber); return yylval.token->type;
"|="		yylval.token = O_CALL_CLASS(Token(), new, yytext, OR_IS, filename, linenumber); return yylval.token->type;
"#="		yylval.token = O_CALL_CLASS(Token(), new, yytext, XOR_IS, filename, linenumber); return yylval.token->type;
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
int yywrap()
{
	yy_delete_buffer(YY_CURRENT_BUFFER);
	fclose(yyin);

	if (!include_stack) {
		yytext = "";
		return 1;
	}
	/* The input_file_name is only allocated for include files, and not for
	   the original input file. Therefore this call to free is after the
	   check on the include level. */
	O_CALL (input_file_name, delete);
	yy_switch_to_buffer(include_stack->buffer_state);
	linenumber = include_stack->current_line;
	input_file_name = include_stack->current_file;
	O_CALL(include_stack, delete);
	return 0;
}

bool try_absolute_path(struct String * file_name, struct String * _absolute_path)
{
	char * absolute_path = realpath(_absolute_path->data, NULL);
	fprintf(stderr, "trying: %s\n", absolute_path);
	if (absolute_path && (yyin = fopen(absolute_path, "r")) != 0) {
		fprintf(stderr, "Found file: %s\n", absolute_path);
		filename = file_name->data;
		free(absolute_path);
		return true;
	}
	if (absolute_path) {
		free(absolute_path);
	}
	return false;
}

void try_search_path(struct String * file_name)
{
	unsigned i;
	
	for (i = 0; i < path->length; i++) {
		struct String * item = (struct String *)O_CALL(path, get, i);
		struct String * full_path = O_CALL_CLASS(String(), new, "%s", item->data);
		O_CALL(full_path, append_str, "/%s.%s", file_name->data, "inc");
		if (try_absolute_path (file_name, full_path)) {
			O_CALL(full_path, delete);
			return;
		}
		O_CALL(full_path, delete);
	}

	if (try_absolute_path (file_name, file_name)) {
		return;
	}
	struct Token * token = O_CALL_CLASS(Token(), new, yytext, IDENTIFIER, filename, linenumber);
	error(token, "cannot open '%s'\n", file_name->data);
	O_CALL (token, delete);
	exit(EXIT_FAILURE);
}

// char * strdup(const char *);
void new_input(struct String * file_name)
{
	O_CALL_CLASS(IncludeStack(), new, linenumber, input_file_name, YY_CURRENT_BUFFER);
	try_search_path(file_name);
	input_file_name = O_CALL_CLASS(String (), new, "%s", file_name->data);
	linenumber = 1;
	yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
	BEGIN(INITIAL);
}
