%{
#include "RefList.h"
#include "Declaration.h"
#include "VarDeclaration.h"
#include "FunDeclaration.h"
#include "ClassDeclaration.h"
#include "ArgDeclaration.h"
#include "InterfaceDeclaration.h"
#include "MacroDeclaration.h"
#include "Statement.h"
#include "IfStatement.h"
#include "DoStatement.h"
#include "WhileStatement.h"
#include "ForStatement.h"
#include "ForEachStatement.h"
#include "ExpressionStatement.h"
#include "CompoundStatement.h"
#include "ReturnStatement.h"
#include "Expression.h"
#include "Token.h"
#include "Type.h"
#include "ArrayType.h"
#include "Scope.h"

  extern void yyerror (const char *);
  extern int yywrap (void);
  extern char *yytext;
  extern struct RefList *global_declarations;
  extern void var_id_decl_set_type(void *_var, va_list *app);

  %}

%union {
  struct RefList * list;
  struct Declaration * declaration;
  struct Statement * statement;
  struct Expression * expression;
  struct Token * token;
  struct CompileObject * object;
  struct Type * type;
}

%token <token> CHAR
%token <token> CHAR_CONSTANT
%token <token> CLASS
%token <token> DO
%token <token> ELSE
%token <token> FLOAT
%token <token> FLOAT_CONSTANT
%token <token> FOR
%token <token> FOREACH
%token <token> IDENTIFIER
%token <token> IF
%token <token> INT
%token <token> INT_CONSTANT
%token <token> INTERFACE
%token <token> MACRO
%token <token> MACRO_IDENTIFIER
%token <token> RETURN
%token <token> STRING_CONSTANT
%token <token> TYPE_IDENTIFIER
%token <token> UNSIGNED
%token <token> VA_ARG /* '...' */
%token <token> WHILE

%type	<list>		input
%type	<list>		opt_declaration_list
%type	<list>		declaration_list
%type	<list>		variable_declaration_list
%type	<declaration>	declaration
%type	<declaration>	function_declaration
%type	<declaration>	class_declaration
%type	<declaration>	interface_declaration
%type	<declaration>	macro_declaration
%type	<declaration>	var_id_decl
%type	<list>		var_id_decl_list
%type	<declaration>	function_header
%type	<list>		formal_arg_list_var
%type	<list>		formal_arg_list
%type	<declaration>	formal_arg
%type	<list>		interface_list
%type	<statement>	statement
%type	<statement>	compound_statement
%type	<statement>	if_statement
%type	<statement>	expression_statement
%type	<statement>	do_statement
%type	<statement>	while_statement
%type	<statement>	for_statement
%type	<statement>	foreach_statement
%type	<statement>	return_statement
%type	<list>		compound_content_list
%type	<list>		opt_compound_content_list
%type	<type>		type
%type	<list>		interface_method_declaration_list
%type	<expression>	expression
%type	<expression>	constant
%type	<list>		actual_arg_list
%type	<list>		opt_actual_arg_list
%type	<token>	string_constant
%type	<list>		macro_identifier_list

%left		<token>	','
%right		<token>	'='
%left		<token>	OR '|' 
%left		<token>	AND '&'
%left		<token>	EQ NEQ
%nonassoc	<token>	'<' LEQ '>' GEQ
%left		<token> XOR '#'
%left		<token>	SHIFTL SHIFTR
%left		<token>	'+' '-'
%left		<token>	'*' '/' '%' '^'
%right		<token>	'!' UNARY_MINUS UNARY_PLUS
%left			'(' '[' '.'

%start input
%expect 1
%%

input
:	declaration_list
{
  global_declarations = $1;
}
;

opt_declaration_list
:	declaration_list
|
	/* empty */
{
  $$ = O_CALL_CLASS(RefList(), new, 0, Declaration());
}
;

declaration_list
:	declaration_list declaration
{
  O_CALL(current_scope, declare, $2);
  O_CALL($$, append, $2);
}
|	declaration 
{
  O_CALL(current_scope, declare, $1);
  struct RefList * result = O_CALL_CLASS(RefList(), new, 8, Declaration()); 
  O_CALL(result, append, $1); 
  $$ = result;
}
|	declaration_list variable_declaration_list
{
  $$ = O_CALL($1, merge, $2);
}
|	variable_declaration_list
;

declaration
:	function_declaration
|	class_declaration
|	interface_declaration
|	macro_declaration
;

variable_declaration_list
:	type var_id_decl_list ';' 
{
  O_CALL($2, map_args, var_id_decl_set_type, $1);
  $$ = $2;
}
;

var_id_decl_list
:	var_id_decl_list ',' var_id_decl
{
  O_CALL($$, append, $3);
}
|	var_id_decl
{
  struct RefList * result = O_CALL_CLASS(RefList(), new, 8, Declaration());
  O_CALL(result, append, $1);
  $$ = result;
}
;

var_id_decl
:	IDENTIFIER
{
  struct Declaration * result = O_CALL_CLASS(VarDeclaration(), new, $1, NULL);
  O_CALL(current_scope, declare, result);
  $$ = result;
}
|	IDENTIFIER '=' expression
{
  struct Declaration * result = O_CALL_CLASS(VarDeclaration(), new, $1, $3);
  O_CALL(current_scope, declare, result);
  $$ = result;
}
;

function_declaration
:	function_header statement
{
  struct FunDeclaration * decl = o_cast($1, FunDeclaration());
  decl->body = $2;
}
;

function_header
:	type IDENTIFIER '(' 
{ 
  O_CALL_CLASS(Scope(), new); 
}
formal_arg_list_var ')'
{
  struct Declaration * result = O_CALL_CLASS(FunDeclaration(), new, $2, $1, $5, NULL);
  $$ = result;
}
;

formal_arg_list_var
:	formal_arg_list ',' VA_ARG
{
  struct ArgDeclaration * arg = O_CALL_CLASS(ArgDeclaration(), new, NULL, $3);
  O_CALL($$, append, arg);
}
|	formal_arg_list
|	VA_ARG
{
  $$ = O_CALL_CLASS(RefList(), new, 8, ArgDeclaration());
  struct ArgDeclaration * arg = O_CALL_CLASS(ArgDeclaration(), new, NULL, $1);
  O_CALL($$, append, arg);
}
|
	/* empty */
{
  $$ = O_CALL_CLASS(RefList(), new, 8, ArgDeclaration());
}
;

formal_arg_list
:	formal_arg_list ',' formal_arg
{
  
  O_CALL($$, append, $3);
}
|	formal_arg
{
  struct RefList * result = O_CALL_CLASS(RefList(), new, 8, ArgDeclaration());
  O_CALL(result, append, $1);
  $$ = result;
}
;

formal_arg
:	type IDENTIFIER
{
  $$ = O_CALL_CLASS(ArgDeclaration(), new, $2, $1);
}
;

class_declaration
:	CLASS TYPE_IDENTIFIER ':' TYPE_IDENTIFIER '<' interface_list '>' '{' opt_declaration_list '}'
{
  $$ = O_CALL_CLASS(ClassDeclaration(), new, $2, $4, $6, $9);
}
|	CLASS TYPE_IDENTIFIER ':' TYPE_IDENTIFIER '{' opt_declaration_list '}'
{
  $$ = O_CALL_CLASS(ClassDeclaration(), new, $2, $4, NULL, $6);
}
;

interface_list
:	interface_list ',' TYPE_IDENTIFIER
{
  O_CALL($$, append, $3);
}
|	TYPE_IDENTIFIER
{
  $$ = O_CALL_CLASS(RefList(), new, 8, Token());
  O_CALL($$, append, $1);
}
;

statement
:	compound_statement
|	if_statement
|	expression_statement
|	do_statement
|	while_statement
|	for_statement
|	foreach_statement
|	return_statement
;

compound_statement
:	'{' opt_compound_content_list '}'
{
  $$ = O_CALL_CLASS(CompoundStatement(), new, $2);
}
;

opt_compound_content_list
:	compound_content_list
|	/* empty */
{
  $$ = O_CALL_CLASS(RefList(), new, 0, CompileObject());
}
;

compound_content_list
:	compound_content_list statement
{
  O_CALL($$, append, $2);
}
|	compound_content_list declaration
{
  O_CALL($$, append, $2);
}
|	compound_content_list variable_declaration_list
{
  O_CALL($$, merge, $2);
}
|	statement
{
  struct RefList * result = O_CALL_CLASS(RefList(), new, 8, CompileObject());
  O_CALL(result, append, $1);
  $$ = result;
}
|	declaration
{
  struct RefList * result = O_CALL_CLASS(RefList(), new, 8, CompileObject());
  O_CALL(result, append, $1);
  $$ = result;
}
|	variable_declaration_list
;

if_statement
:	IF '(' expression ')' statement
{
  $$ = O_CALL_CLASS(IfStatement(), new, $3, $5, NULL);
}
|	IF '(' expression ')' statement ELSE statement
{
  $$ = O_CALL_CLASS(IfStatement(), new, $3, $5, $7);
}
;

expression_statement
:	expression ';'
{
  $$ = O_CALL_CLASS(ExpressionStatement(), new, $1);
}
;

do_statement
:	DO statement WHILE expression ';'
{
  $$ = O_CALL_CLASS(DoStatement(), new, $2, $4);
}
;

while_statement
:	WHILE '(' expression ')' statement
{
  $$ = O_CALL_CLASS(WhileStatement(), new, $3, $5);
}
;

for_statement
:	FOR '(' expression ';' expression ';' expression ')' statement
{
  $$ = O_CALL_CLASS(ForStatement(), new, $3, $5, $7, $9);
}
;

foreach_statement
:	FOREACH '(' IDENTIFIER ':' expression ')' statement
{
  $$ = O_CALL_CLASS(ForEachStatement(), new, $3, $5, $7);
}
;

return_statement
:	RETURN expression ';'
{
  $$ = O_CALL_CLASS(ReturnStatement(), new, $2);
}
;

interface_declaration
:	INTERFACE TYPE_IDENTIFIER '<' interface_list '>' '{' interface_method_declaration_list '}'
{
  $$ = O_CALL_CLASS(InterfaceDeclaration(), new, $2, $4, $7);
}
|	INTERFACE TYPE_IDENTIFIER '{' interface_method_declaration_list '}'
{
  $$ = O_CALL_CLASS(InterfaceDeclaration(), new, $2, NULL, $4);
}
;

interface_method_declaration_list
:	interface_method_declaration_list function_header ';'
|	function_header ';'
{
  struct RefList * result = O_CALL_CLASS(RefList(), new, 8, Declaration());
  O_CALL(result, append, $1);
  $$ = result;
}
;

type
:	TYPE_IDENTIFIER
{
  $$ = O_CALL_CLASS(Type(), new, $1);
}
|	INT
{
  $$ = O_CALL_CLASS(Type(), new, $1);
}
|	UNSIGNED
{
  $$ = O_CALL_CLASS(Type(), new, $1);
}
|	FLOAT
{
  $$ = O_CALL_CLASS(Type(), new, $1);
}
|	CHAR
{
  $$ = O_CALL_CLASS(Type(), new, $1);
}
|	type '[' ']'
{
  $$ = O_CALL_CLASS(ArrayType(), new, $1);
}
;

expression
:	constant
|	IDENTIFIER { $$ = O_CALL_CLASS(Expression(), new, $1, NULL, NULL); }
|	expression '(' opt_actual_arg_list ')'
|	expression '[' expression ']'
|	expression '.' expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	expression '+' expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	expression '-' expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	expression '/' expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	expression '*' expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	expression '^' expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	expression '%' expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	expression '&' expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	expression '|' expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	expression '#' expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	expression '=' expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	expression AND expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	expression OR expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	expression XOR expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	expression EQ expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	expression NEQ expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	expression '<' expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	expression '>' expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	expression LEQ expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	expression GEQ expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	expression SHIFTR expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	expression SHIFTL expression { $$ = O_CALL_CLASS(Expression(), new, $1, $<token>2, $3); }
|	'-' expression %prec UNARY_MINUS { $$ = O_CALL_CLASS(Expression(), new, $2, $1, NULL); }
|	'+' expression %prec UNARY_PLUS { $$ = O_CALL_CLASS(Expression(), new, $2, $1, NULL); }
|	'!' expression { $$ = O_CALL_CLASS(Expression(), new, $2, $1, NULL); }
|	'(' expression ')' { $$ = $2; }
;

constant
:	CHAR_CONSTANT { $$ = O_CALL_CLASS(Expression(), new, $1, NULL, NULL); }
|	FLOAT_CONSTANT { $$ = O_CALL_CLASS(Expression(), new, $1, NULL, NULL); }
|	INT_CONSTANT { $$ = O_CALL_CLASS(Expression(), new, $1, NULL, NULL); }
|	string_constant { $$ = O_CALL_CLASS(Expression(), new, $1, NULL, NULL); }
;

opt_actual_arg_list
:	actual_arg_list
|	/* empty */
{
  $$ = O_CALL_CLASS(RefList(), new, 8, Expression());
}
;

actual_arg_list
:	actual_arg_list ',' expression
{
  O_CALL($$, append, $3);

}
|	expression
{
  $$ = O_CALL_CLASS(RefList(), new, 8, Expression());
  O_CALL($$, append, $1);
}
;

string_constant
:	string_constant STRING_CONSTANT
{
  O_CALL($1->name, append, $2->name);
  O_CALL($2, delete);
}
|	STRING_CONSTANT
;

macro_declaration
:	MACRO IDENTIFIER '(' macro_identifier_list ')' statement
{
  $$ = O_CALL_CLASS(MacroDeclaration(), new, $2, $4, $6);
}
;

macro_identifier_list
:	macro_identifier_list ',' IDENTIFIER
{
  O_CALL($$, append, $3);
}
|	IDENTIFIER
{
  $$ = O_CALL_CLASS(RefList(), new, 8, Token());
  O_CALL($$, append, $1);
}
;

%%
void yyerror (const char * msg)
{
  error (yylval.token, "%s near \"%s\"\n", msg, yytext);
}
int yywrap ()
{
  return 1;
}
int parse ()
{
  O_CALL_CLASS(Scope(), new);
  return yyparse ();
}
void var_id_decl_set_type(void *_var, va_list *app)
{
  struct VarDeclaration * var = O_CAST(_var, VarDeclaration());
  O_CALL(var, set_type, va_arg(*app, struct Type *));
}
