/*
 * carbon Compiler and programming language for libco2
 *
 * Copyright (C) 2011 Peterpaul Taekele Klein Haneveld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
%{
  // #include "Compiler.h"
  /* Declarations */
#include "ArgumentDeclaration.h"
#include "ClassDeclaration.h"
#include "ConstructorDeclaration.h"
#include "Declaration.h"
#include "DestructorDeclaration.h"
#include "FunctionDeclaration.h"
#include "InterfaceDeclaration.h"
#include "InterfaceMethodDefinition.h"
#include "MacroDeclaration.h"
#include "StructDeclaration.h"
#include "TypeDeclaration.h"
#include "VariableDeclaration.h"
  /* Statements */
#include "BreakStatement.h"
#include "CaseStatement.h"
#include "CatchStatement.h"
#include "CompoundStatement.h"
#include "ContinueStatement.h"
#include "DeleteStatement.h"
#include "DoStatement.h"
#include "ExpressionStatement.h"
#include "FinallyStatement.h"
  /* #include "ForEachStatement.h" */
#include "ForStatement.h"
#include "IfStatement.h"
#include "ReturnStatement.h"
#include "Statement.h"
#include "SwitchStatement.h"
#include "TryStatement.h"
#include "ThrowStatement.h"
#include "WhileStatement.h"
  /* Expressions */
#include "BinaryExpression.h"
#include "CastExpression.h"
#include "ConditionalBinaryExpression.h"
#include "ConditionalExpression.h"
#include "Expression.h"
#include "FunctionCallExpression.h"
#include "IsOfExpression.h"
#include "NestedExpression.h"
#include "NewExpression.h"
#include "NullExpression.h"
#include "SizeExpression.h"
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
#include "CompositeScope.h"
#include "Token.h"
#include "co2/String.h"

  extern char *yytext;
  extern void yyerror(const char *);
  extern int yywrap(void);
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
  struct IScope * scope;
}

%token <token> _BREAK
%token <token> _CASE
%token <token> _CATCH
%token <token> _CHAR
%token <token> _CHAR_CONSTANT
%token <token> _CLASS
%token <token> _CONTINUE
%token <token> _DEFAULT
%token <token> _DO
%token <token> _DOUBLE
%token <token> _DELETE
%token <token> _ELSE
%token <token> _FINALLY
%token <token> _FLOAT
%token <token> _FLOAT_CONSTANT
%token <token> _FOR
 /* %token <token> _FOREACH */
%token <token> _GET_VA_ARG
%token <token> _IDENTIFIER
%token <token> _IF
%token <token> _IMPORT
%token <token> _INT
%token <token> _INT_CONSTANT
%token <token> _INTERFACE
%token <token> _NEW
%token <token> _NULL_
%token <token> _RETURN
%token <token> _SELF
%token <token> _SIZEOF
%token <token> _STRING_CONSTANT
%token <token> _STRUCT
%token <token> _SUPER
%token <token> _SWITCH
%token <token> _THROW
%token <token> _TRY
%token <token> _TYPEDEF
%token <token> _TYPE_IDENTIFIER
%token <token> _UNSIGNED
 /* '...' */
%token <token> _VA_ARG
%token <token> _VA_LIST__
%token <token> _VOID
%token <token> _WHILE

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
%type	<object>	implemented_interface_method
%type	<list>		implemented_interface_methods
%type	<declaration>	interface_declaration
%type	<declaration>	interface_header
%type	<list>		interface_list
%type	<list>		interface_method_declaration_list
%type	<list>		opt_formal_argument_list
%type	<declaration>	struct_declaration
%type	<list>		struct_declaration_body
%type	<declaration>	type_declaration
%type	<declaration>	variable_declaration_id
%type	<list>		variable_declaration_id_list
%type	<list>		variable_declaration_list
 /* Statements */
%type	<statement>	break_statement
%type	<list>		case_content_list
%type	<statement>	case_statement
%type	<list>		case_statement_list
%type	<list>		case_statement_list_with_default
%type	<statement>	catch_statement
%type	<list>		catch_statement_list
%type	<list>		compound_content
%type	<list>		compound_content_list
%type	<statement>	compound_statement
%type	<statement>	continue_statement
%type	<statement>	default_case
%type	<statement>	delete_statement
%type	<statement>	do_statement
%type	<statement>	expression_statement
 /* %type	<statement>	foreach_statement */
%type	<statement>	for_statement
%type	<statement>	if_statement
%type	<statement>	return_statement
%type	<statement>	statement
%type	<statement>	switch_statement
%type	<statement>	throw_statement
%type	<statement>	try_statement
%type	<statement>	while_statement
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
%type	<token>		identifier

 /* Solve shift-reduce conflict for if-else */
%nonassoc	_IFX
%nonassoc	_ELSE

 /* Solve shift-reduce conflict for constructor, in combination with '(' */
%nonassoc	_CONSTRUCTX

%nonassoc	_CATCHX
%nonassoc	_CATCH
%nonassoc	_FINALLY

%nonassoc	CLASSX

%left		<token>	','
%right		<token>	'=' _INCREASE _DECREASE _MULTIPLY _DIVIDE _POWER _REMINDER _AND_IS _OR_IS _XOR_IS
%left		<token>	_OR '|' 
%left		<token>	_AND '&'
%left		<token>	_EQ _NEQ _IS_OF
%nonassoc	<token>	'<' _LEQ '>' _GEQ
%left		<token> _XOR '#'
%left		<token>	_SHIFTL _SHIFTR
%left		<token>	'+' '-'
%left		<token>	'*' '/' '%' '^'
%right		<token>	'!' _UNARY_MINUS _UNARY_PLUS _ADDRESS_OF _DEREFERENCE
 /* Solve shift-reduce conflict for casts */
%right		_CASTX
%left		<token> ':' '?'
%left		<token>	'(' '[' '.'

%start input
%expect 0

%%

input
:	global_declaration_list
;

global_declaration_list
:	global_declaration_list declaration_list_content
{
  O_CALL_IF (List, current_file->declarations, merge, $2);
}
|	/* empty */
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
|	function_header ';'
{
  O_CALL_IF(IScope, current_scope, leave);
  O_CALL_IF(IScope, current_scope, declare, $1);
  $$ = O_CALL_CLASS(RefList(), new, 8, Declaration()); 
  O_CALL($$, append, $1); 
}
;

declaration
:	function_declaration
{
  O_CALL_IF(IScope, current_scope, declare, $1);
}
|	struct_declaration
|	class_declaration
|	interface_declaration
{
  O_CALL_IF(IScope, current_scope, declare, $1);
}
|	constructor_declaration
{
  O_CALL_IF(IScope, current_scope, declare, $1);
}
|	destructor_declaration
{
  O_CALL_IF(IScope, current_scope, declare, $1);
}
|	type_declaration
{
  O_CALL_IF(IScope, current_scope, declare, $1);
}
;

type_declaration
:	_TYPEDEF _TYPE_IDENTIFIER '=' type ';'
{
  $$ = O_CALL_CLASS (TypeDeclaration (), new, $2, $4, false);
}
|	_TYPEDEF _TYPE_IDENTIFIER '=' _STRUCT type ';'
{
  $$ = O_CALL_CLASS (TypeDeclaration (), new, $2, $5, true);
}
|	_TYPEDEF _TYPE_IDENTIFIER '=' _IDENTIFIER ';'
{
  struct Type * type = O_CALL_CLASS (PrimitiveType (), new, $4);
  $$ = O_CALL_CLASS (TypeDeclaration (), new, $2, type, false);
}
|	_TYPEDEF _TYPE_IDENTIFIER '=' _STRUCT _IDENTIFIER ';'
{
  struct Type * type = O_CALL_CLASS (PrimitiveType (), new, $5);
  $$ = O_CALL_CLASS (TypeDeclaration (), new, $2, type, true);
}
;

definition_declaration
:	header_file definition
{
  $$ = $2;
  O_CALL($$, mapArgs, declaration_list_set_include_header, $1);
}
|	header_file '{' definition_list '}'
{
  $$ = $3;
  O_CALL($$, mapArgs, declaration_list_set_include_header, $1);
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
:	function_header ';'
{
  O_CALL_IF(IScope, current_scope, leave);
  O_CALL_IF(IScope, current_scope, declare, $1);
  $$ = O_CALL_CLASS(RefList(), new, 8, Declaration());
  O_CALL($$, append, $1);
}
|	declaration 
{
  $$ = O_CALL_CLASS(RefList(), new, 8, Declaration()); 
  O_CALL($$, append, $1); 
}
|	variable_declaration_list
;

header_file
:	'[' _STRING_CONSTANT ']'
{
  $$ = $2;
}
;

variable_declaration_list
:	type variable_declaration_id_list ';' 
{
  O_CALL($2, mapArgs, VariableDeclaration_set_type, $1);
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
:	_IDENTIFIER
{
  $$ = O_CALL_CLASS(VariableDeclaration(), new, $1, NULL);
  O_CALL_IF(IScope, current_scope, declare, $$);
}
|	_CLASS
{
  $$ = O_CALL_CLASS(VariableDeclaration(), new, $1, NULL);
  O_CALL_IF(IScope, current_scope, declare, $$);
}
|	_IDENTIFIER '=' expression
{
  $$ = O_CALL_CLASS(VariableDeclaration(), new, $1, $3);
  O_CALL_IF(IScope, current_scope, declare, $$);
}
;

function_declaration
:	function_header statement
{
  struct FunctionDeclaration * decl = O_CAST($1, FunctionDeclaration());
  decl->body = O_CALL($2, retain);
  O_CALL_IF(IScope, current_scope, leave);
}
;

function_header
:	type _IDENTIFIER '(' 
{ 
  O_CALL_CLASS(Scope(), new, argument_scope_type, $2); 
}
formal_argument_list_var ')' implemented_interface_methods
{
  struct FunctionType * type = O_CALL_CLASS(FunctionType(), new_ctor, _FunctionType_ctor_from_decl, $1, $5);
  struct FunctionDeclaration * decl = O_CALL_CLASS(FunctionDeclaration(), new, $2, type, $5, NULL);
  decl->implemented_methods = O_CALL($7, retain);
  $$ = decl;
}
;

implemented_interface_methods
:	implemented_interface_methods implemented_interface_method
{
  O_CALL ($1, append, $2);
}
|	/* empty */
{
  $$ = O_CALL_CLASS (RefList (), new, 8, InterfaceMethodDefinition ());
}
;

implemented_interface_method
:	',' _TYPE_IDENTIFIER
{
  $$ = O_CALL_CLASS (InterfaceMethodDefinition (), new, $2, NULL);
}
|	',' _TYPE_IDENTIFIER '.' _IDENTIFIER
{
  $$ = O_CALL_CLASS (InterfaceMethodDefinition (), new, $2, $4);
}
;

formal_argument_list_var
:	formal_argument_list ',' _VA_ARG
{
  struct Type * type = O_CALL_CLASS(PrimitiveType(), new, $3);
  struct ArgumentDeclaration * arg = O_CALL_CLASS(ArgumentDeclaration(), new, $3, type);
  O_CALL($$, append, arg);
}
|	opt_formal_argument_list
|	_VA_ARG
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
:	type _IDENTIFIER
{
  $$ = O_CALL_CLASS(ArgumentDeclaration(), new, $2, $1);
  O_CALL_IF(IScope, current_scope, declare, $$);
}
;

struct_declaration
:	_STRUCT identifier
{
  $<scope>$ = O_CALL_CLASS (Scope (), new, struct_scope_type, $2);
}
'{' struct_declaration_body '}'
{
  O_CALL_IF (IScope, current_scope, leave);
  $$ = O_CALL_CLASS (StructDeclaration (), new, $2, $<scope>3, $5);
  O_CALL_IF (IScope, current_scope, declare, $$);
}
;

identifier
: _TYPE_IDENTIFIER
| _IDENTIFIER
;

struct_declaration_body
:	struct_declaration_body variable_declaration_list
{
  O_CALL ($1, merge, $2);
}
|	/* empty */
{
  $$ = O_CALL_CLASS (RefList (), new, 8, VariableDeclaration ());
}
;

class_declaration
:	class_header 
{
  O_CALL_IF(IScope, current_scope, declare, $1);
  struct ClassDeclaration * decl = O_CAST($1, ClassDeclaration());
  decl->member_scope = O_CALL_CLASS(Scope(), new, class_scope_type, decl->name);
  decl->member_scope = O_CALL_CLASS (CompositeScope (), new, decl->member_scope);
}
'{' declaration_list '}'
{
  O_CALL_IF(IScope, current_scope, leave);
  struct ClassDeclaration * decl = O_CAST($1, ClassDeclaration());
  decl->members = O_CALL($4, retain);
  O_CALL(decl->members, mapArgs, Declaration_set_class_decl, decl);
  O_CALL_IF (IScope, decl->member_scope, set_parent, NULL);
  $$ =(struct Declaration *) decl;
}
;

class_header
:	_CLASS _TYPE_IDENTIFIER ':' _TYPE_IDENTIFIER interface_list
{
  O_CALL ($1, delete);
  $$ = O_CALL_CLASS(ClassDeclaration(), new, $2, $4, $5);
}
|	_CLASS _TYPE_IDENTIFIER interface_list
{
  O_CALL ($1, delete);
  $$ = O_CALL_CLASS(ClassDeclaration(), new, $2, NULL, $3);
}
;

interface_list
:	interface_list ',' _TYPE_IDENTIFIER
{
  O_CALL($$, append, $3);
}
|	/* empty */
{
  $$ = O_CALL_CLASS(RefList(), new, 8, Token());
}
;

statement
:	compound_statement
|	if_statement
|	expression_statement
|	do_statement
|	while_statement
|	for_statement
	/* |	foreach_statement */
|	return_statement
|	delete_statement
|	try_statement
|	throw_statement
|	break_statement
|	continue_statement
|	switch_statement
;

compound_statement
:	'{' 
{
  O_CALL_CLASS(Scope(), new, compound_scope_type, NULL);
}
compound_content_list '}'
{
  O_CALL_IF(IScope, current_scope, leave);
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

try_statement
:	_TRY statement catch_statement_list %prec _CATCHX
{
  $$ = O_CALL_CLASS (TryStatement (), new, $2, $3, NULL);
}
|	_TRY statement catch_statement_list _FINALLY statement
{
  struct Statement * final = O_CALL_CLASS (FinallyStatement (), new, $5);
  $$ = O_CALL_CLASS (TryStatement (), new, $2, $3, final);
}
;

catch_statement_list
:	catch_statement_list catch_statement
{
  O_CALL ($1, append, $2);
}
|	/* empty */
{
  $$ = O_CALL_CLASS (RefList (), new, 8, CatchStatement ());
}
;

catch_statement
:	_CATCH 
{
  $<scope>$ = O_CALL_CLASS (Scope (), new, catch_scope_type, NULL);
}
'(' formal_argument ')' statement
{
  O_CALL_IF (IScope, current_scope, leave);
  $$ = O_CALL_CLASS (CatchStatement (), new, $<scope>2, $4, $6);
}
;

throw_statement
:	_THROW expression ';'
{
  $$ = O_CALL_CLASS (ThrowStatement (), new, $2);
}
;

if_statement
:	_IF '(' expression ')' statement %prec _IFX
{
  $$ = O_CALL_CLASS(IfStatement(), new, $3, $5, NULL);
}
|	_IF '(' expression ')' statement _ELSE statement
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
:	_DO statement _WHILE expression ';'
{
  $$ = O_CALL_CLASS(DoStatement(), new, $2, $4);
}
;

while_statement
:	_WHILE '(' expression ')' statement
{
  $$ = O_CALL_CLASS(WhileStatement(), new, $3, $5);
}
;

for_statement
:	_FOR '(' expression ';' expression ';' expression ')' statement
{
  $$ = O_CALL_CLASS(ForStatement(), new, $3, $5, $7, $9);
}
;

return_statement
:	_RETURN expression ';'
{
  $$ = O_CALL_CLASS(ReturnStatement(), new, $2);
}
|	_RETURN ';'
{
  $$ = O_CALL_CLASS(ReturnStatement(), new, NULL);
}
;

delete_statement
:	_DELETE expression ';'
{
  $$ = O_CALL_CLASS(DeleteStatement(), new, $2);
}
;

break_statement
:	_BREAK ';'
{
  $$ = O_CALL_CLASS(BreakStatement(), new);
}
;

continue_statement
:	_CONTINUE ';'
{
  $$ = O_CALL_CLASS(ContinueStatement(), new);
}
;

switch_statement
:	_SWITCH '(' expression ')' '{' case_statement_list_with_default '}'
{
  $$ = O_CALL_CLASS (SwitchStatement (), new, $3, $6);
}
;

case_statement_list_with_default
:	case_statement_list default_case
{
  O_CALL ($$, append, $2);
}
|	case_statement_list
;


case_statement_list
:	case_statement_list case_statement
{
  O_CALL ($$, append, $2);
}
|	/* empty */
{
  $$ = O_CALL_CLASS (RefList (), new, 0, Statement ());
}
;

case_statement
:	_CASE constant ':' case_content_list
{
  $$ = O_CALL_CLASS (CaseStatement (), new, $2, $4);
}
;

case_content_list
:	case_content_list statement
{
  O_CALL ($$, append, $2);
}
|	/* empty */
{
  $$ = O_CALL_CLASS (RefList (), new, 8, Statement ());
}
;

default_case
:	_DEFAULT ':' case_content_list
{
  $$ = O_CALL_CLASS (CaseStatement (), new, NULL, $3);
}
;

interface_declaration
:	interface_header 
{
  struct InterfaceDeclaration * decl = O_CAST($1, InterfaceDeclaration());
  decl->member_scope = O_CALL_CLASS(Scope(), new, interface_scope_type, decl->name);
  decl->member_scope = O_CALL_CLASS (CompositeScope (), new, decl->member_scope);
}
'{' interface_method_declaration_list '}'
{
  O_CALL_IF(IScope, current_scope, leave);
  struct InterfaceDeclaration * decl = O_CAST($1, InterfaceDeclaration());
  decl->members = O_CALL($4, retain);
}
;

interface_header
:	_INTERFACE _TYPE_IDENTIFIER interface_list 
{
  $$ = O_CALL_CLASS(InterfaceDeclaration(), new, $2, $3);
}
;

interface_method_declaration_list
:	interface_method_declaration_list function_header ';'
{
  O_CALL_IF(IScope, current_scope, leave);
  O_CALL_IF(IScope, current_scope, declare, $2);
  O_CALL($1, append, $2);
}
|	interface_method_declaration_list function_declaration
{
  O_CALL_IF(IScope, current_scope, declare, $2);
  O_CALL($1, append, $2);
}
|	/* empty */
{
  $$ = O_CALL_CLASS(RefList(), new, 8, Declaration());
}
;

type
:	_TYPE_IDENTIFIER %prec _CONSTRUCTX
{
  $$ = O_CALL_CLASS(ObjectType(), new, $1, NULL);
}
|	_INT
{
  $$ = O_CALL_CLASS(PrimitiveType(), new, $1);
}
|	_UNSIGNED
{
  $$ = O_CALL_CLASS(PrimitiveType(), new, $1);
}
|	_FLOAT
{
  $$ = O_CALL_CLASS(PrimitiveType(), new, $1);
}
|	_DOUBLE
{
  $$ = O_CALL_CLASS(PrimitiveType(), new, $1);
}
|	_CHAR
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
|	_VA_LIST__
{
  $$ = O_CALL_CLASS(PrimitiveType(), new, $1);
}
|	_VOID
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
|	_IDENTIFIER { $$ = O_CALL_CLASS(TokenExpression(), new, $1); }
|	_TYPE_IDENTIFIER '.' _CLASS { $$ = O_CALL_CLASS(TokenExpression(), new, $1); }
|	_CLASS { $$ = O_CALL_CLASS(TokenExpression(), new, $1); }
|	_SELF { $$ = O_CALL_CLASS(TokenExpression(), new, $1); }
|	_VA_ARG { $$ = O_CALL_CLASS(TokenExpression(), new, $1); }
|	expression '(' opt_actual_argument_list ')' { $$ = O_CALL_CLASS(FunctionCallExpression(), new, $1, $3); }
|	expression '[' expression ']' { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression '.' expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression '?' '.' expression { $$ = O_CALL_CLASS(ConditionalBinaryExpression(), new, $1, $<token>3, $4); }
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
|	expression _AND expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression _OR expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression _XOR expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression _EQ expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression _NEQ expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression '<' expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression '>' expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression _LEQ expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression _GEQ expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression _SHIFTR expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression _SHIFTL expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression _INCREASE expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression _DECREASE expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression _MULTIPLY expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression _DIVIDE expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression _POWER expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression _REMINDER expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression _AND_IS expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression _OR_IS expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	expression _XOR_IS expression { $$ = O_CALL_CLASS(BinaryExpression(), new, $1, $<token>2, $3); }
|	'-' expression %prec _UNARY_MINUS { $$ = O_CALL_CLASS(UnaryExpression(), new, $1, $2); }
|	'+' expression %prec _UNARY_PLUS { $$ = O_CALL_CLASS(UnaryExpression(), new, $1, $2); }
|	'!' expression { $$ = O_CALL_CLASS(UnaryExpression(), new, $1, $2); }
|	'&' expression %prec _ADDRESS_OF { $$ = O_CALL_CLASS(UnaryExpression(), new, $1, $2); }
|	'*' expression %prec _DEREFERENCE { $$ = O_CALL_CLASS(UnaryExpression(), new, $1, $2); }
|	'(' expression ')' { $$ = O_CALL_CLASS(NestedExpression(), new, $2); }
|	_NEW type '[' expression ']' { $$ = O_CALL_CLASS(NewExpression(), new, $2, $4); }
|	_NEW type '(' opt_actual_argument_list ')' { $$ = O_CALL_CLASS(NewExpression(), new, $2, $4); }
|	_NEW type '.' _IDENTIFIER '(' opt_actual_argument_list ')' 
{
  struct TokenExpression * token_expr = O_CALL_CLASS(TokenExpression(), new, $4);
  struct NewExpression * new_expr = O_CALL_CLASS(NewExpression(), new, $2, $6);
  O_CALL(new_expr, set_ctor_name, token_expr);
  $$ =(struct Expression *) new_expr;
}
|	_SUPER '(' opt_actual_argument_list ')' { $$ = O_CALL_CLASS(SuperExpression(), new, $1, NULL, $3); }
|	_SUPER '.' _IDENTIFIER '(' opt_actual_argument_list ')' { $$ = O_CALL_CLASS(SuperExpression(), new, $1, $3, $5); }
|	_GET_VA_ARG '(' type ')' { $$ = O_CALL_CLASS(VarArgExpression(), new, $3, NULL); }
|	_GET_VA_ARG '(' expression ',' type ')' { $$ = O_CALL_CLASS(VarArgExpression(), new, $5, $3); }
|	_SIZEOF '(' type ')' { $$ = O_CALL_CLASS(SizeExpression(), new, $3); }
|	'(' type ')' expression %prec _CASTX { $$ = O_CALL_CLASS(CastExpression(), new, $2, $4); }
|	expression '?' expression ':' expression { $$ = O_CALL_CLASS(ConditionalExpression (), new, $1, $3, $5); }
<<<<<<< HEAD
|	expression _IS_OF _TYPE_IDENTIFIER {
=======
|	expression IS_OF TYPE_IDENTIFIER %prec CLASSX {
>>>>>>> master
  struct TokenExpression * expr = O_CALL_CLASS(TokenExpression(), new, $3);
  $$ = O_CALL_CLASS(IsOfExpression (), new, $1, expr);
}
|	expression _IS_OF expression { $$ = O_CALL_CLASS(IsOfExpression (), new, $1, $3); }
;

constant
:	_CHAR_CONSTANT { $$ = O_CALL_CLASS(TokenExpression(), new, $1); }
|	_FLOAT_CONSTANT { $$ = O_CALL_CLASS(TokenExpression(), new, $1); }
|	_INT_CONSTANT { $$ = O_CALL_CLASS(TokenExpression(), new, $1); }
|	string_constant { $$ = O_CALL_CLASS(TokenExpression(), new, $1); }
|	_NULL_ { $$ = O_CALL_CLASS(NullExpression(), new, $1); }
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
:	string_constant _STRING_CONSTANT
{
  O_CALL($1->name, append, $2->name);
  O_CALL($2, delete);
}
|	_STRING_CONSTANT
;

constructor_declaration
:	_TYPE_IDENTIFIER '(' 
{ 
  $<token>$ = O_CALL_CLASS(Token(), new_ctor, _Token_ctor_fromToken, $1, "ctor", _IDENTIFIER);
  O_CALL_CLASS(Scope(), new, argument_scope_type, $<token>$);
}
formal_argument_list_var ')' statement
{
  $$ = O_CALL_CLASS(ConstructorDeclaration(), new, $<token>3, $1, $4, $6);
  O_CALL_IF(IScope, current_scope, leave);
}
|	_TYPE_IDENTIFIER '.' _IDENTIFIER '(' 
{ 
  O_CALL_CLASS(Scope(), new, argument_scope_type, $3); 
}
formal_argument_list_var ')' statement
{
  $$ = O_CALL_CLASS(ConstructorDeclaration(), new, $3, $1, $6, $8);
  O_CALL_IF(IScope, current_scope, leave);
}
;

destructor_declaration
:	'~' _TYPE_IDENTIFIER '(' ')' statement
{
  struct Token * dtor_name = O_CALL_CLASS(Token(), new_ctor, _Token_ctor_fromToken, $2, "dtor", _IDENTIFIER);
  $$ = O_CALL_CLASS(DestructorDeclaration(), new, dtor_name, $2, $5);
}
;

%%
void yyerror(const char * msg)
{
  error(yylval.token, "%s near '%s'\n", msg, yytext);
}
int parse()
{
  {
    struct ScopeType * _tmp = O_CALL_CLASS (ScopeType (), new);
    O_CALL (_tmp, delete);
  }

  global_scope = O_CALL_CLASS(Scope(), new, global_scope_type, NULL);
  int result = yyparse();
  O_CALL_IF(IScope, current_scope, leave);
  if(result == 0)
    {
      struct ScopeType *scope_type = O_BRANCH_CALL_IF (IScope, current_scope, get_type);
      assertTrue(current_scope == NULL, "current_scope(%d) is not NULL", scope_type);
      // O_CALL(parsed_file, parse_imports);
    }
  return result;
}
void VariableDeclaration_set_type(void *_decl, va_list *app)
{
  struct VariableDeclaration * decl = O_CAST(_decl, VariableDeclaration());
  O_CALL(decl, set_type, va_arg (*app, struct Type *));
}
void Declaration_set_class_decl(void *_decl, va_list *app)
{
  struct Declaration * decl = O_CAST(_decl, Declaration());
  O_CALL(decl, set_class_decl, va_arg(*app, void *));
}
