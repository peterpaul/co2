%{
#include "co2/Compiler.h"
#include "grammar.h"
#include "co2/Token.h"
#include "co2/RefList.h"
#include "co2/IncludeStack.h"
#include "co2/String.h"
#include "co2/List.h"
#include "co2/File.h"
#include "co2/error.h"
#include <limits.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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
"break"		return BREAK;
"catch"		return CATCH;
"char"		yylval.token = O_CALL_CLASS(Token(), new, yytext, CHAR, current_file, linenumber); return yylval.token->type;
"class"		return CLASS;
"continue"	return CONTINUE;
"delete"	return DELETE;
"do"		return DO;
"double"	yylval.token = O_CALL_CLASS(Token(), new, yytext, DOUBLE, current_file, linenumber); return yylval.token->type;
"else"		return ELSE;
"finally"	return FINALLY;
"float"		yylval.token = O_CALL_CLASS(Token(), new, yytext, FLOAT, current_file, linenumber); return yylval.token->type;
"for"		return FOR;
"if"		return IF;
"import"	return IMPORT;
"int"		yylval.token = O_CALL_CLASS(Token(), new, yytext, INT, current_file, linenumber); return yylval.token->type;
"interface"	return INTERFACE;
"new"		return NEW;
"null"		yylval.token = O_CALL_CLASS(Token(), new, yytext, _NULL, current_file, linenumber); return yylval.token->type;
"package"	return PACKAGE;
"return"	return RETURN;
"self"		yylval.token = O_CALL_CLASS(Token(), new, yytext, SELF, current_file, linenumber); return yylval.token->type;
"sizeof"	return SIZEOF;
"struct"	return STRUCT;
"super"		yylval.token = O_CALL_CLASS(Token(), new, yytext, SUPER, current_file, linenumber); return yylval.token->type;
"throw"		return THROW;
"try"		return TRY;
"typedef"	return TYPEDEF;
"unsigned"	yylval.token = O_CALL_CLASS(Token(), new, yytext, UNSIGNED, current_file, linenumber); return yylval.token->type;
"va_arg"	return GET_VA_ARG;
"va_list"	yylval.token = O_CALL_CLASS(Token(), new, yytext, VA_LIST, current_file, linenumber); return yylval.token->type;
"void"		yylval.token = O_CALL_CLASS(Token(), new, yytext, VOID, current_file, linenumber); return yylval.token->type;
"while"		return WHILE;
"..."		yylval.token = O_CALL_CLASS(Token(), new, yytext, VA_ARG, current_file, linenumber); return yylval.token->type;
"&&"		yylval.token = O_CALL_CLASS(Token(), new, yytext, AND, current_file, linenumber); return yylval.token->type;
"||"		yylval.token = O_CALL_CLASS(Token(), new, yytext, OR, current_file, linenumber); return yylval.token->type;
"=="		yylval.token = O_CALL_CLASS(Token(), new, yytext, EQ, current_file, linenumber); return yylval.token->type;
"!="		yylval.token = O_CALL_CLASS(Token(), new, yytext, NEQ, current_file, linenumber); return yylval.token->type;
"##"		yylval.token = O_CALL_CLASS(Token(), new, yytext, XOR, current_file, linenumber); return yylval.token->type;
"<="		yylval.token = O_CALL_CLASS(Token(), new, yytext, LEQ, current_file, linenumber); return yylval.token->type;
">="		yylval.token = O_CALL_CLASS(Token(), new, yytext, GEQ, current_file, linenumber); return yylval.token->type;
">>"		yylval.token = O_CALL_CLASS(Token(), new, yytext, SHIFTR, current_file, linenumber); return yylval.token->type;
"<<"		yylval.token = O_CALL_CLASS(Token(), new, yytext, SHIFTL, current_file, linenumber); return yylval.token->type;
"+="		yylval.token = O_CALL_CLASS(Token(), new, yytext, INCREASE, current_file, linenumber); return yylval.token->type;
"-="		yylval.token = O_CALL_CLASS(Token(), new, yytext, DECREASE, current_file, linenumber); return yylval.token->type;
"*="		yylval.token = O_CALL_CLASS(Token(), new, yytext, MULTIPLY, current_file, linenumber); return yylval.token->type;
"/="		yylval.token = O_CALL_CLASS(Token(), new, yytext, DIVIDE, current_file, linenumber); return yylval.token->type;
"^="		yylval.token = O_CALL_CLASS(Token(), new, yytext, POWER, current_file, linenumber); return yylval.token->type;
"%="		yylval.token = O_CALL_CLASS(Token(), new, yytext, REMINDER, current_file, linenumber); return yylval.token->type;
"&="		yylval.token = O_CALL_CLASS(Token(), new, yytext, AND_IS, current_file, linenumber); return yylval.token->type;
"|="		yylval.token = O_CALL_CLASS(Token(), new, yytext, OR_IS, current_file, linenumber); return yylval.token->type;
"#="		yylval.token = O_CALL_CLASS(Token(), new, yytext, XOR_IS, current_file, linenumber); return yylval.token->type;
{INT_CONST}	yylval.token = O_CALL_CLASS(Token(), new, yytext, INT_CONSTANT, current_file, linenumber); return yylval.token->type;
{FLOAT_CONST}	yylval.token = O_CALL_CLASS(Token(), new, yytext, FLOAT_CONSTANT, current_file, linenumber); return yylval.token->type;
{IDENTIFIER}	yylval.token = O_CALL_CLASS(Token(), new, yytext, IDENTIFIER, current_file, linenumber); return yylval.token->type;
{TYPE_IDENT}	yylval.token = O_CALL_CLASS(Token(), new, yytext, TYPE_IDENTIFIER, current_file, linenumber); return yylval.token->type;
{MACRO_IDENT}	yylval.token = O_CALL_CLASS(Token(), new, yytext, MACRO_IDENTIFIER, current_file, linenumber); return yylval.token->type;
{CHAR_CONST}	yylval.token = O_CALL_CLASS(Token(), new, yytext, CHAR_CONSTANT, current_file, linenumber); return yylval.token->type;
{CHAR_CONST_ERR}  yylval.token = O_CALL_CLASS(Token(), new, yytext, CHAR_CONSTANT, current_file, linenumber); error(yylval.token, "invalid character constant %s", yytext); return yylval.token->type;
{STRING_CONST}	yylval.token = O_CALL_CLASS(Token(), new, yytext, STRING_CONSTANT, current_file, linenumber); return yylval.token->type;
\n		linenumber++; /* count lines */
[ \t]		/* eat whitespace */
[\(\)\]\{\}\;\:\,]	return yytext[0]; /* no Token for these characters */
.		yylval.token = O_CALL_CLASS(Token(), new, yytext, yytext[0], current_file, linenumber); return yylval.token->type;
%%
int yywrap()
{
	yy_delete_buffer(YY_CURRENT_BUFFER);
	fclose(yyin);

	if (!include_stack) {
		yytext = "";
		return 1;
	}
	yy_switch_to_buffer(include_stack->buffer_state);
	linenumber = include_stack->current_line;
	current_file = include_stack->current_file;
	O_CALL(include_stack, delete);
	return 0;
}

/**
 * Checks whether file already exists in the hash map.
 * When it already exists, this function deletes file, and return NULL. If
 * the file doesn't exist, the file is added to the hash map, opened for
 * reading (yyin) and is returned.
 *
 * @param file The file to validate.
 * @return file when the file has not been parsed yet, NULL otherwise
 */
struct File * validate_file(struct File * file)
{
	struct File * existing_file = O_CALL (get_hash_map (), get, file->absolute_path->data);
	if (existing_file)
	{
		/* printf ("skipping already parsed file: %s\n", file->absolute_path->data); */
		O_CALL (file, delete);
		return NULL;
	}
	O_CALL (get_hash_map (), add, file->absolute_path->data, file);
	yyin = fopen (file->absolute_path->data, "r");
	return file;
}

struct File * try_absolute_path(struct String * file_name, struct String * _absolute_path)
{
	struct stat st;
	char * _real_path = realpath (_absolute_path->data, NULL);
	struct String * absolute_path = O_CALL_CLASS (String (), new, "%s", _real_path);
	free (_real_path);
	if (absolute_path->data && stat (absolute_path->data, &st) == 0) {
		/* fprintf (stderr, "Found file: %s\n", absolute_path->data); */
		return O_CALL_CLASS (File (), new, file_name, absolute_path);
	}
	O_CALL (absolute_path, delete);
	return NULL;
}

struct File * try_search_path(struct String * file_name)
{
	unsigned i;
	struct File * file = NULL;
	for (i = 0; i < path->length; i++) {
		struct String * item = (struct String *)O_CALL(path, get, i);
		struct String * full_path = O_CALL_CLASS(String(), new, "%s", item->data);
		O_CALL(full_path, append_str, "/%s.%s", file_name->data, "co2");

		/* fprintf (stderr, "trying: %s\n", full_path->data); */
		if ((file = try_absolute_path (file_name, full_path))) {
			O_CALL(full_path, delete);
			return validate_file (file);
		}
		O_CALL(full_path, delete);
	}

	if ((file = try_absolute_path (file_name, file_name))) {
		return validate_file (file);
	}
	struct Token * token = O_CALL_CLASS(Token(), new, yytext, IDENTIFIER, current_file, linenumber);
	error(token, "cannot open '%s'\n", file_name->data);
	O_CALL (token, delete);
	exit(EXIT_FAILURE);
	return NULL;
}

// char * strdup(const char *);
void new_input(struct String * file_name)
{
	struct File * new_file = try_search_path(file_name);
	if (new_file) {
		O_CALL_CLASS(IncludeStack(), new, linenumber, current_file, YY_CURRENT_BUFFER);
		current_file = new_file;
		O_CALL (include_stack->current_file->file_dependencies, append, current_file);
		linenumber = 1;
		yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
	}
	BEGIN(INITIAL);
	yytext = "";
}

int linenumber = 1;
struct IncludeStack * include_stack = NULL;

#define O_SUPER RefObject()

O_IMPLEMENT (IncludeStack, void *, ctor, (void *_self, va_list * app))
{
  struct IncludeStack *self = O_CAST (_self, IncludeStack ());
  self = O_SUPER->ctor (self, app);
  self->parent = include_stack;
  include_stack = self;
  self->current_line = va_arg (*app, unsigned);
  self->current_file = O_RETAIN_ARG (File);
  self->buffer_state = va_arg (*app, YY_BUFFER_STATE);
  return self;
}

O_IMPLEMENT (IncludeStack, void *, dtor, (void *_self))
{
  struct IncludeStack *self = O_CAST (_self, IncludeStack ());
  O_CALL (self->current_file, release);
  include_stack = self->parent;
  return O_SUPER->dtor (self);
}

O_OBJECT (IncludeStack, RefObject);
O_OBJECT_METHOD (IncludeStack, ctor);
O_OBJECT_METHOD (IncludeStack, dtor);
O_END_OBJECT
