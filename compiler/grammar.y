%{
  /* Declarations */
#include "ArgumentDeclaration.h"
#include "ClassDeclaration.h"
#include "ConstructorDeclaration.h"
#include "Declaration.h"
#include "DestructorDeclaration.h"
#include "FunctionDeclaration.h"
#include "InterfaceDeclaration.h"
#include "MacroDeclaration.h"
#include "VariableDeclaration.h"
  /* Statements */
#include "CompoundStatement.h"
#include "DeleteStatement.h"
#include "DoStatement.h"
#include "ExpressionStatement.h"
#include "ForEachStatement.h"
#include "ForStatement.h"
#include "IfStatement.h"
#include "ReturnStatement.h"
#include "Statement.h"
#include "WhileStatement.h"
  /* Expressions */
#include "BinaryExpression.h"
#include "Expression.h"
#include "FunctionCallExpression.h"
#include "NestedExpression.h"
#include "NewExpression.h"
#include "SuperExpression.h"
#include "TokenExpression.h"
#include "UnaryExpression.h"
#include "VarArgExpression.h"
  /* Types */
#include "ArrayType.h"
#include "FunctionType.h"
#include "ObjectType.h"
#include "PrimitiveType.h"
#include "Type.h"
  /* Support */
#include "File.h"
#include "io.h"
#include "Path.h"
#include "RefList.h"
#include "Scope.h"
#include "Token.h"

  extern char *yytext;
  extern struct File * parsed_file;
  extern void yyerror (const char *);
  extern int yywrap (void);
  extern void VariableDeclaration_set_type(void *_var, va_list *app);
  extern void Declaration_set_class_decl(void *_decl, va_list *app);

  %}

%union {
  struct CompileObject * object;
  struct Declaration * declaration;
  struct Expression * expression;
  struct File * file;
  struct Path * path;
  struct RefList * list;
  struct Statement * statement;
  struct Token * token;
  struct Type * type;
}

%token <token> CHAR
%token <token> CHAR_CONSTANT
%token <token> CLASS
%token <token> DO
%token <token> DELETE
%token <token> ELSE
%token <token> FLOAT
%token <token> FLOAT_CONSTANT
%token <token> FOR
%token <token> FOREACH
%token <token> GET_VA_ARG
%token <token> IDENTIFIER
%token <token> IF
%token <token> IMPORT
%token <token> INT
%token <token> INT_CONSTANT
%token <token> INTERFACE
%token <token> MACRO
%token <token> MACRO_IDENTIFIER
%token <token> NEW
%token <token> PACKAGE
%token <token> RETURN
%token <token> SELF
%token <token> STRING_CONSTANT
%token <token> SUPER
%token <token> TYPE_IDENTIFIER
%token <token> UNSIGNED
%token <token> VA_ARG /* '...' */
%token <token> VOID
%token <token> WHILE

%type	<file>		input
 /* Declarations */
%type	<declaration>	class_declaration
%type	<declaration>	class_header
%type	<declaration>	constructor_declaration
%type	<declaration>	declaration
%type	<list>		declaration_list
%type	<list>		declaration_list_content
%type	<list>		definition_declaration
%type	<list>		definition_list
%type	<list>		definition
%type	<declaration>	destructor_declaration
%type	<declaration>	formal_argument
%type	<list>		formal_argument_list
%type	<list>		formal_argument_list_var
%type	<declaration>	function_declaration
%type	<declaration>	function_header
%type	<token>		header_file
%type	<declaration>	interface_declaration
%type	<declaration>	interface_header
%type	<list>		interface_list
%type	<list>		interface_method_declaration_list
%type	<declaration>	macro_declaration
%type	<list>		macro_identifier_list
%type	<list>		opt_formal_argument_list
%type	<declaration>	variable_declaration_id
%type	<list>		variable_declaration_id_list
%type	<list>		variable_declaration_list
 /* Statements */
%type	<list>		compound_content
%type	<list>		compound_content_list
%type	<statement>	compound_statement
%type	<statement>	delete_statement
%type	<statement>	do_statement
%type	<statement>	expression_statement
%type	<statement>	foreach_statement
%type	<statement>	for_statement
%type	<statement>	if_statement
%type	<statement>	return_statement
%type	<statement>	statement
%type	<statement>	while_statement
 /* Imports */
%type	<token>		identifier
%type	<path>		import
%type	<list>		import_list
%type	<list>		import_path
%type	<path>		package
 /* Types */
%type	<list>		opt_type_list
%type	<type>		type
%type	<list>		type_list
 /* Expressions */
%type	<list>		actual_argument_list
%type	<expression>	constant
%type	<expression>	expression
%type	<list>		opt_actual_argument_list
%type	<token>		string_constant

 /* Solve shift-reduce conflict for if-else */
%nonassoc	IFX
%nonassoc	ELSE

 /* Solve shift-reduce conflict for constructor, in combination with '(' */
%nonassoc	CONSTRUCTX

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
%left		<token>	'(' '[' '.'

%start input
%expect 0

%%

input
:	package import_list declaration_list
{
  $$ = O_CALL_CLASS(File(), new, $1, $2, $3);
  parsed_file = $$;
}
;

package
:	PACKAGE import_path ';'
{
  $$ = O_CALL_CLASS(Path(), new, $2);
}
;

import_list
:	import_list import
{
  O_CALL($1, append, $2);
}
|	/* empty */
{
  $$ = O_CALL_CLASS(RefList(), new, 1, Path());
}
;

import
:	IMPORT import_path ';'
{
  $$ = O_CALL_CLASS(Path(), new, $2);
}
;

import_path
:	import_path '.' identifier
{
  O_CALL($1, append, $3);
}
|	identifier
{
  $$ = O_CALL_CLASS(RefList(), new, 8, Token());
  O_CALL($$, append, $1);
}
;

identifier
:	IDENTIFIER
|	TYPE_IDENTIFIER
;

declaration_list
:	declaration_list declaration_list_content
{
  $$ = O_CALL($1, merge, $2);
}
|	/* empty */
{
  $$ = O_CALL_CLASS(RefList(), new, 8, Declaration());
}
;

declaration_list_content
:	declaration 
{
  $$ = O_CALL_CLASS(RefList(), new, 8, Declaration()); 
  O_CALL($$, append, $1); 
}
|	variable_declaration_list
|	definition_declaration
;

declaration
:	function_declaration
{
  O_CALL(current_scope, declare, $1);
}
|	class_declaration
|	interface_declaration
{
  O_CALL(current_scope, declare, $1);
}
|	macro_declaration
{
  O_CALL(current_scope, declare, $1);
}
|	constructor_declaration
{
  O_CALL(current_scope, declare, $1);
}
|	destructor_declaration
{
  O_CALL(current_scope, declare, $1);
}
;

definition_declaration
:	header_file definition
{
  $$ = $2;
  O_CALL($$, map_args, Declaration_list_set_include_header, $1);
}
|	header_file '{' definition_list '}'
{
  $$ = $3;
  O_CALL($$, map_args, Declaration_list_set_include_header, $1);
}
;

definition_list
:	definition_list definition
{
  O_CALL($1, merge, $2);
}
|	definition
;

definition
:	variable_declaration_list
|	function_header ';'
{
  O_CALL(current_scope, leave);
  O_CALL(current_scope, declare, $1);
  $$ = O_CALL_CLASS(RefList(), new, 8, Declaration());
  O_CALL($$, append, $1);
}
;

header_file
:	'[' STRING_CONSTANT ']'
{
  $$ = $2;
}
;

variable_declaration_list
:	type variable_declaration_id_list ';' 
{
  O_CALL($2, map_args, VariableDeclaration_set_type, $1);
  $$ = $2;
}
;

variable_declaration_id_list
:	variable_declaration_id_list ',' variable_declaration_id
{
  O_CALL($$, append, $3);
}
|	variable_declaration_id
{
  $$ = O_CALL_CLASS(RefList(), new, 8, Declaration());
  O_CALL($$, append, $1);
}
;

variable_declaration_id
:	IDENTIFIER
{
  $$ = O_CALL_CLASS(VariableDeclaration(), new, $1, NULL);
  O_CALL(current_scope, declare, $$);
}
|	IDENTIFIER '=' expression
{
  $$ = O_CALL_CLASS(VariableDeclaration(), new, $1, $3);
  O_CALL(current_scope, declare, $$);
}
;

function_declaration
:	function_header statement
{
  struct FunctionDeclaration * decl = O_CAST($1, FunctionDeclaration());
  decl->body = O_CALL($2, retain);
  O_CALL(current_scope, leave);
}
;

function_header
:	type IDENTIFIER '(' 
{ 
  O_CALL_CLASS(Scope(), new, ARGUMENT_SCOPE, $2); 
}
formal_argument_list_var ')'
{
  struct FunctionType * type = O_CALL_CLASS(FunctionType(), new_ctor, _FunctionType_ctor_from_decl, $1, $5);
  $$ = O_CALL_CLASS(FunctionDeclaration(), new, $2, type, $5, NULL);
}
;

formal_argument_list_var
:	formal_argument_list ',' VA_ARG
{
  struct Type * type = O_CALL_CLASS(PrimitiveType(), new, $3);
  struct ArgumentDeclaration * arg = O_CALL_CLASS(ArgumentDeclaration(), new, $3, type);
  O_CALL($$, append, arg);
}
|	opt_formal_argument_list
|	VA_ARG
{
  $$ = O_CALL_CLASS(RefList(), new, 8, ArgumentDeclaration());
  struct Type * type = O_CALL_CLASS(PrimitiveType(), new, $1);
  struct ArgumentDeclaration * arg = O_CALL_CLASS(ArgumentDeclaration(), new, $1, type);
  O_CALL($$, append, arg);
}
;

opt_formal_argument_list
:	formal_argument_list
|	/* empty */
{
  $$ = O_CALL_CLASS(RefList(), new, 8, ArgumentDeclaration());
}
;

formal_argument_list
:	formal_argument_list ',' formal_argument
{
  O_CALL($$, append, $3);
}
|	formal_argument
{
  $$ = O_CALL_CLASS(RefList(), new, 8, ArgumentDeclaration());
  O_CALL($$, append, $1);
}
;

formal_argument
:	type IDENTIFIER
{
  $$ = O_CALL_CLASS(ArgumentDeclaration(), new, $2, $1);
  O_CALL(current_scope, declare, $$);
}
;

class_declaration
:	class_header 
{
  O_CALL(current_scope, declare, $1);
  struct ClassDeclaration * decl = O_CAST($1, ClassDeclaration());
  decl->member_scope = O_CALL_CLASS(Scope(), new, CLASS_SCOPE, decl->name);
}
'{' declaration_list '}'
{
  O_CALL(current_scope, leave);
  struct ClassDeclaration * decl = O_CAST($1, ClassDeclaration());
  decl->members = O_CALL($4, retain);
  O_CALL(decl->members, map_args, Declaration_set_class_decl, decl);
  decl->member_scope->parent = NULL;
  $$ = (struct Declaration *) decl;
}

class_header
:	CLASS TYPE_IDENTIFIER ':' TYPE_IDENTIFIER '<' interface_list '>'
{
  $$ = O_CALL_CLASS(ClassDeclaration(), new, $2, $4, $6);
}
|	CLASS TYPE_IDENTIFIER ':' TYPE_IDENTIFIER
{
  $$ = O_CALL_CLASS(ClassDeclaration(), new, $2, $4, NULL);
}
|	CLASS TYPE_IDENTIFIER '<' interface_list '>'
{
  $$ = O_CALL_CLASS(ClassDeclaration(), new, $2, NULL, $4);
}
|	CLASS TYPE_IDENTIFIER
{
  $$ = O_CALL_CLASS(ClassDeclaration(), new, $2, NULL, NULL);
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
|	delete_statement
;

compound_statement
:	'{' 
{
  O_CALL_CLASS(Scope(), new, COMPOUND_SCOPE, NULL);
}
compound_content_list '}'
{
  O_CALL(current_scope, leave);
  $$ = O_CALL_CLASS(CompoundStatement(), new, $3);
}
;

compound_content_list
:	compound_content_list compound_content
{
  O_CALL($$, merge, $2);
}
|	/* empty */
{
  $$ = O_CALL_CLASS(RefList(), new, 8, CompileObject());
}
;

compound_content
:	statement
{
  $$ = O_CALL_CLASS(RefList(), new, 8, CompileObject());
  O_CALL($$, append, $1);
}
|	declaration
{
  $$ = O_CALL_CLASS(RefList(), new, 8, CompileObject());
  O_CALL($$, append, $1);
}
|	variable_declaration_list
{
  $$ = O_CALL_CLASS(RefList(), new, 8, CompileObject());
  O_CALL($$, merge, $1);
}
;

if_statement
:	IF '(' expression ')' statement %prec IFX
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
|	RETURN ';'
{
  $$ = O_CALL_CLASS(ReturnStatement(), new, NULL);
}
;

delete_statement
:	DELETE expression ';'
{
  $$ = O_CALL_CLASS(DeleteStatement(), new, $2);
}
;

interface_declaration
:	interface_header 
{
  struct InterfaceDeclaration * decl = O_CAST($1, InterfaceDeclaration());
  decl->member_scope = O_CALL_CLASS(Scope(), new, INTERFACE_SCOPE, decl->name);
}
'{' interface_method_declaration_list '}'
{
  O_CALL(current_scope, leave);
  struct InterfaceDeclaration * decl = O_CAST($1, InterfaceDeclaration());
  decl->members = O_CALL($4, retain);
}
;

interface_header
:	INTERFACE TYPE_IDENTIFIER '<' interface_list '>' 
{
  $$ = O_CALL_CLASS(InterfaceDeclaration(), new, $2, $4);
}
|	INTERFACE TYPE_IDENTIFIER 
{
  $$ = O_CALL_CLASS(InterfaceDeclaration(), new, $2, NULL);
}
;

interface_method_declaration_list
:	interface_method_declaration_list function_header ';'
{
  O_CALL(current_scope, leave);
  O_CALL(current_scope, declare, $2);
  O_CALL($1, append, $2);
}
|	function_header ';'
{
  O_CALL(current_scope, leave);
  $$ = O_CALL_CLASS(RefList(), new, 8, Declaration());
  O_CALL(current_scope, declare, $1);
  O_CALL($$, append, $1);
}
;

type
:	TYPE_IDENTIFIER %prec CONSTRUCTX
{
  $$ = O_CALL_CLASS(ObjectType(), new, $1, NULL);
}
|	INT
{
  $$ = O_CALL_CLASS(PrimitiveType(), new, $1);
}
|	UNSIGNED
{
  $$ = O_CALL_CLASS(PrimitiveType(), new, $1);
}
|	FLOAT
{
  $$ = O_CALL_CLASS(PrimitiveType(), new, $1);
}
|	CHAR
{
  $$ = O_CALL_CLASS(PrimitiveType(), new, $1);
}
|	type '[' ']'
{
  $$ = O_CALL_CLASS(ArrayType(), new, $1);
}
|	type '*'
{
  $$ = O_CALL_CLASS(ArrayType(), new, $1);
}
|	VOID
{
  $$ = O_CALL_CLASS(PrimitiveType(), new, $1);
}
|	type '(' '*' ')' '(' opt_type_list ')'
{
	$$ = O_CALL_CLASS(FunctionType(), new, $1, $6);
}
;

opt_type_list
:	type_list
|	/* empty */
{
	$$ = O_CALL_CLASS(RefList(), new, 8, Type());
}
;

type_list
:	type_list ',' type
{
	O_CALL($1, append, $3);
}
|	type
{
	$$ = O_CALL_CLASS(RefList(), new, 8, Type());
	O_CALL($$, append, $1);
}
;

expression
:	constant
|	IDENTIFIER { $$ = O_CALL_CLASS(TokenExpression(), new, $1); }
|	SELF { $$ = O_CALL_CLASS(TokenExpression(), new, $1); }
|	expression '(' opt_actual_argument_list ')' { $$ = O_CALL_CLASS(FunctionCallExpression(), new, $1, $3); }
|	expression '[' expression ']' { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression '.' expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression '+' expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression '-' expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression '/' expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression '*' expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression '^' expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression '%' expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression '&' expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression '|' expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression '#' expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression '=' expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression AND expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression OR expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression XOR expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression EQ expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression NEQ expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression '<' expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression '>' expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression LEQ expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression GEQ expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression SHIFTR expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression SHIFTL expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	'-' expression %prec UNARY_MINUS { $$ = O_CALL_CLASS(UnaryExpression(), new, $1, $2); }
|	'+' expression %prec UNARY_PLUS { $$ = O_CALL_CLASS(UnaryExpression(), new, $1, $2); }
|	'!' expression { $$ = O_CALL_CLASS(UnaryExpression(), new, $1, $2); }
|	'(' expression ')' { $$ = O_CALL_CLASS(NestedExpression(), new, $2); }
|	NEW type '[' expression ']' { $$ = O_CALL_CLASS(NewExpression(), new, $2, $4); }
|	NEW type '(' opt_actual_argument_list ')' { $$ = O_CALL_CLASS(NewExpression(), new, $2, $4); }
|	NEW type '.' IDENTIFIER '(' opt_actual_argument_list ')' 
{
  struct TokenExpression * token_expr = O_CALL_CLASS(TokenExpression(), new, $4);
  struct NewExpression * new_expr = O_CALL_CLASS(NewExpression(), new, $2, $6);
  O_CALL(new_expr, set_ctor_name, token_expr);
  $$ = (struct Expression *) new_expr;
}
|	SUPER '(' opt_actual_argument_list ')' 
{ 
  $$ = O_CALL_CLASS(SuperExpression(), new, $1, NULL, $3);
}
|	SUPER '.' IDENTIFIER '(' opt_actual_argument_list ')' { $$ = O_CALL_CLASS(SuperExpression(), new, $1, $3, $5); }
|	GET_VA_ARG '(' type ')' { $$ = O_CALL_CLASS(VarArgExpression(), new, $3); }
;

constant
:	CHAR_CONSTANT { $$ = O_CALL_CLASS(TokenExpression(), new, $1); }
|	FLOAT_CONSTANT { $$ = O_CALL_CLASS(TokenExpression(), new, $1); }
|	INT_CONSTANT { $$ = O_CALL_CLASS(TokenExpression(), new, $1); }
|	string_constant { $$ = O_CALL_CLASS(TokenExpression(), new, $1); }
;

opt_actual_argument_list
:	actual_argument_list
|	/* empty */
{
  $$ = O_CALL_CLASS(RefList(), new, 8, Expression());
}
;

actual_argument_list
:	actual_argument_list ',' expression
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

constructor_declaration
:	TYPE_IDENTIFIER '(' 
{ 
  $<token>$ = O_CALL_CLASS(Token(), new, "ctor", IDENTIFIER, filename, $1->line);
  O_CALL_CLASS(Scope(), new, ARGUMENT_SCOPE, $<token>$);
}
opt_formal_argument_list ')' statement
{
  $$ = O_CALL_CLASS(ConstructorDeclaration(), new, $<token>3, $1, $4, $6);
  O_CALL(current_scope, leave);
}
|	TYPE_IDENTIFIER '.' IDENTIFIER '(' 
{ 
  O_CALL_CLASS(Scope(), new, ARGUMENT_SCOPE, $3); 
}
opt_formal_argument_list ')' statement
{
  $$ = O_CALL_CLASS(ConstructorDeclaration(), new, $3, $1, $6, $8);
  O_CALL(current_scope, leave);
}
;

destructor_declaration
:	'~' TYPE_IDENTIFIER '(' ')' statement
{
  struct Token * dtor_name = O_CALL_CLASS(Token(), new, "dtor", IDENTIFIER, filename, $2->line);
  $$ = O_CALL_CLASS(DestructorDeclaration(), new, dtor_name, $2, $5);
}
;

%%
void yyerror (const char * msg)
{
  error (yylval.token, "%s near '%s'\n", msg, yytext);
}
int parse ()
{
  global_scope = O_CALL_CLASS(Scope(), new, GLOBAL_SCOPE, NULL);
  int result = yyparse ();
  O_CALL(current_scope, leave);
  if (result == 0)
    {
      assertTrue(current_scope == NULL, "current_scope (%d) is not NULL", current_scope->type);
      O_CALL(parsed_file, parse_imports);
    }
  return result;
}
void VariableDeclaration_set_type(void *_decl, va_list *app)
{
  struct VariableDeclaration * decl = O_CAST(_decl, VariableDeclaration());
  O_CALL(decl, set_type, va_arg(*app, struct Type *));
}
void Declaration_set_class_decl(void *_decl, va_list *app)
{
  struct Declaration * decl = O_CAST(_decl, Declaration());
  O_CALL(decl, set_class_decl, va_arg(*app, void *));
}
