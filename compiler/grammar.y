%{
  include "Object.h"
%}

%token CHAR
%token CHAR_CONSTANT
%token CLASS
%token DO
%token ELSE
%token FLOAT
%token FLOAT_CONSTANT
%token FOR
%token FOREACH
%token IDENTIFIER
%token IF
%token INT
%token INT_CONSTANT
%token INTERFACE
%token MACRO
%token MACRO_IDENTIFIER
%token STRING_CONSTANT
%token TYPE_IDENTIFIER
%token VA_ARG /* '...' */
%token WHILE

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
;

declaration_list
:	declaration_list declaration
|	declaration
;

declaration
:	variable_declaration 
|	function_declaration
|	class_declaration
|	interface_declaration
|	macro_declaration
;

variable_declaration
:	type var_id_decl_list ';'
;

var_id_decl_list
:	var_id_decl_list ',' var_id_decl
|	var_id_decl
;

var_id_decl
:	IDENTIFIER 
|	IDENTIFIER '=' constant
;

function_declaration
:	function_header statement
;

function_header
:	type IDENTIFIER '(' formal_arg_list_var ')'
;

formal_arg_list_var
:	formal_arg_list ',' VA_ARG
|	formal_arg_list
|	VA_ARG
;

formal_arg_list
:	formal_arg_list formal_arg
|	formal_arg
;

formal_arg
:	type IDENTIFIER
;

class_declaration
:	CLASS TYPE_IDENTIFIER ':' TYPE_IDENTIFIER '<' interface_list '>' '{' declaration_list '}'
|	CLASS TYPE_IDENTIFIER ':' TYPE_IDENTIFIER '{' declaration_list '}'
;

interface_list
:	interface_list ',' TYPE_IDENTIFIER
|	TYPE_IDENTIFIER
;

statement
:	compound_statement
|	if_statement
|	expression_statement
|	do_statement
|	while_statement
|	for_statement
|	foreach_statement
|	assignment_statement
;

compound_statement
:	'{' compound_content_list '}'
;

compound_content_list
:	compound_content_list compound_content
|	compound_content
;

compound_content
:	variable_declaration
|	statement
;

if_statement
:	IF '(' expression ')' statement
|	IF '(' expression ')' statement ELSE statement
;

expression_statement
:	expression ';'
;

do_statement
:	DO statement WHILE expression ';'
;

while_statement
:	WHILE '(' expression ')' statement
;

for_statement
:	FOR '(' expression ';' expression ';' expression ')' statement
;

foreach_statement
:	FOREACH '(' IDENTIFIER ':' expression ')' statement
;

assignment_statement
:	expression '=' expression ';'
;

interface_declaration
:	INTERFACE TYPE_IDENTIFIER '<' interface_list '>' '{' interface_method_declaration_list '}'
|	INTERFACE TYPE_IDENTIFIER '{' interface_method_declaration_list '}'
;

interface_method_declaration_list
:	interface_method_declaration_list function_header ';'
|	function_header ';'
;

type
:	TYPE_IDENTIFIER
|	INT
|	FLOAT
|	CHAR
|	type '[' ']'
;

expression
:	constant
|	IDENTIFIER
|	expression '(' actual_arg_list ')'
|	expression '.' expression
|	expression '+' expression
|	expression '-' expression
|	expression '/' expression
|	expression '*' expression
|	expression '^' expression
|	expression '%' expression
|	expression '&' expression
|	expression '|' expression
|	expression '#' expression
|	expression AND expression
|	expression OR expression
|	expression XOR expression
|	expression EQ expression
|	expression NEQ expression
|	expression '<' expression
|	expression '>' expression
|	expression LEQ expression
|	expression GEQ expression
|	expression SHIFTR expression
|	expression SHIFTL expression
|	'-' expression %prec UNARY_MINUS
|	'+' expression %prec UNARY_PLUS
|	'!' expression 
|	'(' expression ')'	
;

constant
:	CHAR_CONSTANT
|	FLOAT_CONSTANT
|	INT_CONSTANT
|	string_constant
;

actual_arg_list
:	actual_arg_list ',' expression
|	expression
;

string_constant
:	string_constant STRING_CONSTANT
|	STRING_CONSTANT
;

macro_declaration
:	MACRO IDENTIFIER '(' macro_identifier_list ')' statement
;

macro_identifier_list
:	macro_identifier_list ',' IDENTIFIER
|	IDENTIFIER
;

%%
