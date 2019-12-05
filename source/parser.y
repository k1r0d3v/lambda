%skeleton  "lalr1.cc"
%require  "3.4"
%debug
%defines
%define  api.parser.class  {Parser}
%define  api.token.constructor
%define  api.value.type  variant
%define  parse.assert

%code  requires  {
//
//  Headers  used  for  token  types  must  be  included  here
//
#include  <deque>
#include  <sstream>
#include  <string>
#include  <ast/ast.hpp>

//  Required  for  %param
namespace  yy  {  class  Driver;  }
}  //  end  %code  requires

%param  {  yy::Driver  *driver  }
%locations
%define  parse.error  verbose

%code  {
//  Driver  include  must  be  here  (YY_DECL  and  YY_DRIVERDATA)
#include  <ast_driver.hpp>

//  Make  the  ast  operations  shorter
#define  MKNODE(T,  ...)  ast::Node::make<ast::T>(__VA_ARGS__)
#define  MKTYPE(T,  ...)  ast::Type::make<ast::T>(__VA_ARGS__)
#define  MKERROR(l, m) yy::Parser::error(l, m); throw  std::runtime_error(m);
}  //  end  %code


%token  END  0  "EOF"
%token  <int>  NUMBER  "number"
%token  <double>  FLOAT  "float"
%token  <std::string>  IDENTIFIER  "identifier"
%token  <std::string>  TYPE_NAME  "type  name"
%token  <std::string>  STRING  "string"

%token  K_PRINT  "print"
%token  K_SUCC  "succ"
%token  K_PRED  "pred"
%token  K_ISZERO  "iszero"
%token  K_TRUE  "true"
%token  K_FALSE  "false"
%token  K_IF  "if"
%token  K_THEN  "then"
%token  K_ELSE  "else"
%token  K_LET  "let"
%token  K_LETREC  "letrec"
%token  K_FIX  "fix"
%token  K_IN  "in"
%token  K_LAMBDA  "lambda"
%token  K_AS  "as"
%token  K_TOP  "Top"
%token  K_BOOL  "Bool"
%token  K_NAT  "Nat"
%token  K_FLOAT  "Float"
%token  K_STR  "Str"
%token  K_UNIT  "Unit"
%token  K_ALIAS  "alias"

%token  S_LPAREN  "("
%token  S_RPAREN  ")"
%token  S_DOT  "."
%token  S_COMMA  ","
%token  S_EQ  "="
%token  S_COLON  ":"
%token  S_COLON2  "::"
%token  S_SEMICOLON  ";"
%token  S_LINE_END  ";;"
%token  S_ARROW  "->"
%token  S_LBRACE  "{"
%token  S_RBRACE  "}"
%token  S_LBRACKET  "["
%token  S_RBRACKET  "]"

/*
  TODO: Check precedences
*/
%right       S_ARROW
%nonassoc    K_THEN
%nonassoc    K_ELSE
%left        S_SEMICOLON
%left        K_FIX K_SUCC K_PRED K_ISZERO K_PRINT
%left        K_AS
%right       S_COLON2
%left        S_DOT
%precedence  S_LPAREN  S_RPAREN
%left        K_IN
%precedence  S_LINE_END

%type  <ast::Node::Pointer>  unit
%type  <ast::Node::Pointer>  natural_constant
%type  <ast::Node::Pointer>  float_constant
%type  <ast::Node::Pointer>  string_constant
%type  <ast::Node::Pointer>  bool_constant
%type  <ast::Node::Pointer>  abstraction
%type  <ast::Node::Pointer>  ascription
//%type  <ast::Node::Pointer>  application /* Shift reduce warnings comes from here */
%type  <ast::Node::Pointer>  identifier
%type  <ast::Node::Pointer>  if_then_else
%type  <ast::Node::Pointer>  let_in
%type  <ast::Node::Pointer>  sequence
%type  <ast::Node::Pointer>  assignment
%type  <ast::Node::Pointer>  fix
%type  <ast::Node::Pointer>  let_rec
%type  <ast::Node::Pointer>  succ
%type  <ast::Node::Pointer>  pred
%type  <ast::Node::Pointer>  iszero
%type  <ast::Node::Pointer>  print
%type  <ast::Node::Pointer>  file
%type  <ast::Node::Pointer>  operator_dot
%type  <ast::Node::Pointer>  alias

%type  <ast::Node::Pointer>  term
%type  <ast::Node::Pointer>  tuple
%type  <ast::Node::Pointer>  list
%type  <ast::List::Pointer>  list_terms
%type  <std::deque<ast::Node::Pointer>> tuple_terms
%type  <ast::List::Pointer> list_concat
%type  <ast::list<ast::Node::Pointer>> list_concat_terms
%type  <ast::Node::Pointer>  register
%type  <std::map<ast::string, ast::Node::Pointer>> register_terms

%type  <ast::Type::Pointer>  type_name
%type  <ast::Type::Pointer>  tuple_type
%type  <std::deque<ast::Type::Pointer>> tuple_type_types
%type  <ast::Type::Pointer>  list_type
%type  <ast::Type::Pointer>  register_type
%type  <std::map<ast::string, ast::Type::Pointer>> register_type_types

%type  <ast::Pattern::Pointer>  pattern
%type  <ast::Pattern::Pointer>  tuple_pattern
%type  <std::deque<ast::Pattern::Pointer>> tuple_pattern_terms
%type  <ast::Pattern::Pointer>  list_pattern
%type  <ast::List::Pointer>  list_pattern_terms
%type  <ast::Pattern::Pointer>  register_pattern
%type  <std::map<ast::string, ast::Pattern::Pointer>> register_pattern_terms

%type  <ast::AST*>  s

%%

s:
  file { YY_DRIVERDATA->setRoot($1); }
;

file:
  END                  { $$ = ast::Node::Pointer(); }
| term S_LINE_END file { $$ = ast::Sequence::join($1, $3); }
| assignment S_LINE_END file  {  $$ = ast::Sequence::join($1,  $3);  }
| alias S_LINE_END file { $$ = ast::Sequence::join($1,  $3); }
;

assignment:
  K_LET pattern S_EQ term { $$ = MKNODE(Declaration, $2, $4); }
| K_LET error S_EQ term { MKERROR(driver->yyLocation(), "This expression is not appropriate to make an assignment") }
| K_LET pattern S_EQ error { MKERROR(driver->yyLocation(), "The value expected is a term") }
;

alias:
  K_ALIAS TYPE_NAME S_COLON type_name { $$ = MKNODE(Alias, $2, $4); }
| K_ALIAS TYPE_NAME S_COLON error { MKERROR(driver->yyLocation(), "The value assigment to alias is not a valid type.") }
| K_ALIAS error S_COLON type_name { MKERROR(driver->yyLocation(), "Not valid characters to the name of alias") }
;

type_name:
  S_LPAREN  type_name  S_RPAREN  {  $$  =  $2;  }
| S_LPAREN  error  S_RPAREN  {  MKERROR(driver->yyLocation(), "Not a valid type.")  }
|  TYPE_NAME  {  $$  =  MKTYPE(UndefinedType,  $1);  }
|  K_BOOL  {  $$  =  MKTYPE(BoolType);  }
|  K_NAT  {  $$  =  MKTYPE(NatType);  }
|  K_FLOAT  {  $$  =  MKTYPE(FloatType);  }
|  K_STR  {  $$  =  MKTYPE(StrType);  }
|  K_UNIT  {  $$  =  MKTYPE(UnitType);  }
|  K_TOP  {  $$  =  MKTYPE(TopType);  }
|  type_name  S_ARROW  type_name  {  $$  =  MKTYPE(ArrowType,  $1,  $3);  }
|  tuple_type  {  $$  =  $1;  }
|  register_type { $$  =  $1; }
|  list_type { $$ = $1; }
;

unit:
  S_LPAREN  S_RPAREN  {  $$  =  MKNODE(Unit);  }
;

natural_constant:
  NUMBER  {  $$  =  MKNODE(NaturalConstant,  $1);  }
;

float_constant:
  FLOAT  { $$  =  MKNODE(FloatConstant, $1);  }
;

string_constant:
  STRING  {  $$  =  MKNODE(StringConstant,  $1);  }
;

bool_constant:
  K_TRUE  {  $$  =  MKNODE(BooleanConstant,  true);  }
|  K_FALSE  {  $$  =  MKNODE(BooleanConstant,  false);  }
;

identifier:
  IDENTIFIER  {  $$  =  MKNODE(Identifier,  $1);  }
;

abstraction:
  K_LAMBDA  IDENTIFIER  S_COLON  type_name  S_DOT term  {  $$  =  MKNODE(Abstraction,  MKNODE(Variable,  $2,  $4),  $6);  }
| K_LAMBDA  error  S_COLON  type_name  S_DOT term { MKERROR(driver->yyLocation(), "Not valid identifier token to abstraction") }
| K_LAMBDA  IDENTIFIER  S_COLON  error S_DOT term {  MKERROR(driver->yyLocation(), "The value assigment to identifier is not a valid type.") }
| K_LAMBDA  IDENTIFIER  S_COLON  type_name  S_DOT error { MKERROR(driver->yyLocation(), "The value expected is a term") }
;
/*
application:
  term term  {  $$  =  MKNODE(Application,  $1,  $2);  }
;*/

ascription:
  term  K_AS  type_name  {  $$  =  MKNODE(Ascription,  $1,  $3);  }
| term  K_AS  error { MKERROR(driver->yyLocation(), "Not valid type to do ascription") }
;

if_then_else:
  K_IF  term  K_THEN  term  K_ELSE  term  {  $$  =  MKNODE(Conditional,  $2,  $4,  $6);  }
;

pattern:
  IDENTIFIER { $$ = MKNODE(Identifier, $1); }
| tuple_pattern { $$ = $1; }
| register_pattern { $$ = $1; }
| list_pattern { $$ = $1; }
;

let_in:
  K_LET  pattern  S_EQ  term  K_IN  term  {  $$  =  MKNODE(LocalDefinition,  $2,  $4,  $6);  }
| K_LET  error  S_EQ  term  K_IN  term  { MKERROR(driver->yyLocation(), "This expression is not appropriate to assignment a term") }
;

fix:
  K_FIX term  {  $$  =  MKNODE(Fix,  $2);  }
;

let_rec:
  K_LETREC IDENTIFIER S_COLON type_name S_EQ term K_IN term  {
    $$  =  MKNODE(LocalDefinition,  MKNODE(Identifier,  $2),  MKNODE(Fix,  MKNODE(Abstraction,  MKNODE(Variable,  $2,  $4),  $6)),  $8);
  }
| K_LETREC IDENTIFIER S_COLON error S_EQ term K_IN term { MKERROR(driver->yyLocation(), "This expression is not appropriate to assignment a term") }
| K_LETREC error S_COLON type_name S_EQ term K_IN term { MKERROR(driver->yyLocation(), "Not valid identifier to let rec expresion") }
;

sequence:
    term  S_SEMICOLON  term  {  $$  =  ast::Sequence::join($1,  $3);  }
;

succ: K_SUCC term {  $$  =  MKNODE(Successor,  $2);  }
;

pred: K_PRED term {  $$  =  MKNODE(Predecessor,  $2);  }
;

iszero: K_ISZERO term {  $$  =  MKNODE(IsZero,  $2);  }
;

print: K_PRINT term {  $$  =  MKNODE(Print,  $2);  }
;

tuple:
  S_LBRACE tuple_terms S_RBRACE { $$ = MKNODE(Tuple, $2.begin(), $2.end()); }
;

tuple_type:
  S_LBRACE tuple_type_types S_RBRACE { $$ = MKTYPE(TupleType, $2.begin(), $2.end()); }
;

tuple_type_types:
  type_name { $$ = { $1 }; }
| error { MKERROR(driver->yyLocation(), "Not valid type assigment to tuple") }
| type_name S_COMMA tuple_type_types  { $3.push_front($1); $$ = $3; }
| error S_COMMA tuple_type_types {MKERROR(driver->yyLocation(), "Not valid type assigment to tuple")}
;

tuple_terms:
  term                      { $$ = { $1 }; }
| term S_COMMA tuple_terms  { $3.push_front($1); $$ = $3; };
;

tuple_pattern:
  S_LBRACE tuple_pattern_terms S_RBRACE { $$ = MKNODE(Tuple, $2.begin(), $2.end()); }
;

tuple_pattern_terms:
  pattern { $$ = { $1 }; }
| error { MKERROR(driver->yyLocation(), "This expression is not appropriate to assignment a tuple element") }
| pattern S_COMMA tuple_pattern_terms  { $3.push_front($1); $$ = $3; };
| error S_COMMA tuple_pattern_terms { MKERROR(driver->yyLocation(), "This expression is not appropriate to assignment a tuple element") }
;

list:
  S_LBRACKET list_terms S_RBRACKET { $$ = $2; }
//| S_LBRACKET term S_RBRACKET { $$ = MKNODE(List, $2, MKNODE(List,ast::list<ast::Node::Pointer>())); }
| S_LBRACKET  S_RBRACKET { $$ = MKNODE(List, ast::list<ast::Node::Pointer>()); }
;

list_terms:
  term { $$ = MKNODE(List, $1, MKNODE(List,ast::list<ast::Node::Pointer>())); }
| term S_COMMA list_terms { $$ = MKNODE(List, $1, $3); }
;

list_concat:
   S_LBRACKET term S_COLON2 list_concat_terms S_RBRACKET { $4.push_back($2); $$ = MKNODE(List, $4); }
//  term S_COLON2 list_concat { $$ = MKNODE(List, $1, ast::Node::cast<ast::List>($3)); }
//| term S_COLON2 list { $$ = MKNODE(List, $1, ast::Node::cast<ast::List>($3)); }
//| term S_COLON2 IDENTIFIER { $$ = MKNODE(List, $1, MKNODE(List, MKNODE(Identifier, $3), MKNODE(List, ast::list<ast::Node::Pointer>()))); }
;

list_concat_terms:
  term { $$ = {$1}; }
| term S_COLON2 list_concat_terms { $3.push_back($1); $$ = $3; }
;

list_pattern:
  S_LBRACKET list_pattern_terms S_RBRACKET { $$ = $2; }
;

list_pattern_terms:
  IDENTIFIER S_COLON2 list_pattern_terms { $$ = MKNODE(List, MKNODE(Identifier, $1), ast::Node::cast<ast::List>($3)); }
| IDENTIFIER { $$ = MKNODE(List, MKNODE(Identifier, $1), MKNODE(List, ast::list<ast::Node::Pointer>())); }
;

list_type:
  S_LBRACKET type_name S_RBRACKET { $$ = MKTYPE(ListType, $2); }
| S_LBRACKET error S_RBRACKET { MKERROR(driver->yyLocation(), "Not valid type to list type") }
;

register:
  S_LBRACE register_terms S_RBRACE { $$ = MKNODE(Register, $2); }
;

register_terms:
  IDENTIFIER S_COLON term { $$ = { {$1, $3} }; }
| error S_COLON term { MKERROR(driver->yyLocation(), "Not valid identifier to register") }
| IDENTIFIER S_COLON term S_COMMA register_terms  { $5.insert({$1, $3}); $$ = $5; }
| error S_COLON term S_COMMA register_terms { MKERROR(driver->yyLocation(), "Not valid identifier to register") }
;

register_type:
  S_LBRACE register_type_types S_RBRACE { $$ = MKTYPE(RegisterType, $2); }
;

register_type_types:
  IDENTIFIER S_COLON type_name { $$ = { {$1, $3} }; }
| error S_COLON type_name { MKERROR(driver->yyLocation(), "Not valid identifier to register type") }
| IDENTIFIER S_COLON error { MKERROR(driver->yyLocation(), "Not valid type to register type") }
| IDENTIFIER S_COLON type_name S_COMMA register_type_types  { $5.insert({$1, $3}); $$ = $5; }
| error S_COLON type_name S_COMMA register_type_types { MKERROR(driver->yyLocation(), "Not valid identifier to register type") }
| IDENTIFIER S_COLON error S_COMMA register_type_types { MKERROR(driver->yyLocation(), "Not valid type to register type") }
;

register_pattern:
  S_LBRACE register_pattern_terms S_RBRACE { $$ = MKNODE(Register, $2); }
;

register_pattern_terms:
  IDENTIFIER S_COLON pattern { $$ = { {$1, $3} }; }
| error S_COLON pattern { MKERROR(driver->yyLocation(), "Not valid identifier to assign register value") }
| IDENTIFIER S_COLON error { MKERROR(driver->yyLocation(), "This expression is not appropriate to assignment a register value") }
| IDENTIFIER S_COLON pattern S_COMMA register_pattern_terms  { $5.insert({$1, $3}); $$ = $5; }
| error S_COLON pattern S_COMMA register_pattern_terms { MKERROR(driver->yyLocation(), "Not valid identifier to assign register value") }
| IDENTIFIER S_COLON error S_COMMA register_pattern_terms { MKERROR(driver->yyLocation(), "This expression is not appropriate to assignment a register value") }
;

operator_dot:
  term S_DOT IDENTIFIER { $$  =  MKNODE(OperatorDot,  $1,  MKNODE(Identifier,  $3)); }
| term S_DOT NUMBER  {  $$  =  MKNODE(OperatorDot,  $1,  MKNODE(NaturalConstant,  $3));  }
| term S_DOT error { MKERROR(driver->yyLocation(), "The method to access the value is not appropriate") }
;

term:
  S_LPAREN  term  S_RPAREN  {  $$  =  $2;  }
|  unit  {  $$  =  $1;  }
|  natural_constant  {  $$  =  $1;  }
|  float_constant  {  $$  =  $1;  }
|  string_constant  {  $$  =  $1;  }
|  bool_constant  {  $$  =  $1;  }
|  abstraction  {  $$  =  $1;  }
|  ascription { $$ = $1; }
//|  application  {  $$  =  $1;  }
|  identifier  {  $$  =  $1;  }
| if_then_else  { $$ = $1; }
| let_in  { $$ = $1; }
| sequence  { $$ = $1; }
| list { $$ = $1;}
| fix  { $$ = $1; }
| let_rec  { $$ = $1; }
| succ  { $$ = $1; }
| pred  { $$ = $1; }
| iszero  { $$ = $1; }
| print  { $$ = $1; }
| tuple  { $$ = $1; }
| register  { $$ = $1; }
| operator_dot { $$ = $1; }
| list_concat  { $$= $1; }
;

%%

/**
  *  Implement  error  function
  */
void  yy::Parser::error(const  location_type  &l,  const  std::string  &message)
{
  std::cout << message << std::endl;
  if( message.find("syntax error,") == std::string::npos )
  {
  	std::cout << "Error in line : " << l.begin.line << " column: " <<  l.begin.column << "-"  << l.end.column << " " << message << std::endl;
  }
}