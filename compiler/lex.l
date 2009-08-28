%{
#include "grammar.tab.h"
#include "Token.h"

int linenumber = 1;
const char * filename;
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
"do"		return DO;
"else"		return ELSE;
"float"		yylval.token = O_CALL_CLASS(Token(), new, yytext, FLOAT, filename, linenumber); return yylval.token->type;
"for"		return FOR;
"if"		return IF;
"int"		yylval.token = O_CALL_CLASS(Token(), new, yytext, INT, filename, linenumber); return yylval.token->type;
"new"		return NEW;
"return"	return RETURN;
"unsigned"	return UNSIGNED;
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
{STRING_CONST}	yylval.token = O_CALL_CLASS(Token(), new, yytext, CHAR_CONSTANT, filename, linenumber); return yylval.token->type;

\n		linenumber++; /* count lines */
[ \t]		/* eat whitespace */
[\(\)\[\]\{\}\;\:\,\.]	return yytext[0]; /* no Token for these characters */
.		yylval.token = O_CALL_CLASS(Token(), new, yytext, yytext[0], filename, linenumber); return yylval.token->type;
%%
