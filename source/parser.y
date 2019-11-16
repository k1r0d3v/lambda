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
}  //  end  %code


%token  END  0  "EOF"
%token  <int>  NUMBER  "number"
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
%token  K_STR  "Str"
%token  K_UNIT  "Unit"

%token  S_LPAREN  "("
%token  S_RPAREN  ")"
%token  S_DOT  "."
%token  S_COMMA  ","
%token  S_EQ  "="
%token  S_COLON  ":"
%token  S_SEMICOLON  ";"
%token  S_LINE_END  ";;"
%token  S_ARROW  "->"
%token  S_LBRACKET  "{"
%token  S_RBRACKET  "}"

/*
  TODO: Check precedences
*/
%right       S_ARROW
%left        S_DOT
%nonassoc    K_THEN
%nonassoc    K_ELSE
%left        S_SEMICOLON
%left        K_AS
%left        K_FIX K_SUCC K_PRED K_ISZERO K_PRINT
%precedence  S_LPAREN  S_RPAREN
%left        K_IN
%precedence  S_LINE_END

%type  <ast::Node::Pointer>  unit
%type  <ast::Node::Pointer>  natural_constant
%type  <ast::Node::Pointer>  string_constant
%type  <ast::Node::Pointer>  bool_constant
%type  <ast::Node::Pointer>  abstraction
%type  <ast::Node::Pointer>  ascription
%type  <ast::Node::Pointer>  application /* Shift reduce warnings comes from here */
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

%type  <ast::Node::Pointer>  term
%type  <ast::Node::Pointer>  tuple
%type  <std::deque<ast::Node::Pointer>> tuple_terms
%type  <ast::Node::Pointer>  register
%type  <std::map<ast::string, ast::Node::Pointer>> register_terms

%type  <ast::Type::Pointer>  type_name
%type  <ast::Type::Pointer>  tuple_type
%type  <std::deque<ast::Type::Pointer>> tuple_type_types
%type  <ast::Type::Pointer>  register_type
%type  <std::map<ast::string, ast::Type::Pointer>> register_type_types

%type  <ast::Pattern::Pointer>  pattern
%type  <ast::Pattern::Pointer>  tuple_pattern
%type  <std::deque<ast::Pattern::Pointer>> tuple_pattern_terms
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
|  assignment S_LINE_END file  {  $$ = ast::Sequence::join($1,  $3);  }
;

assignment:
    K_LET pattern S_EQ term { $$ = MKNODE(Declaration, $2, $4); }
;

type_name:
  S_LPAREN  type_name  S_RPAREN  {  $$  =  $2;  }
|  TYPE_NAME  {  $$  =  MKTYPE(ConstantType,  ast::TypeKind::Composed,  $1);  }
|  K_BOOL  {  $$  =  MKTYPE(BoolType);  }
|  K_NAT  {  $$  =  MKTYPE(NatType);  }
|  K_STR  {  $$  =  MKTYPE(StrType);  }
|  K_UNIT  {  $$  =  MKTYPE(UnitType);  }
|  K_TOP  {  $$  =  MKTYPE(DynType);  }
|  type_name  S_ARROW  type_name  {  $$  =  MKTYPE(ArrowType,  $1,  $3);  }
|  tuple_type  {  $$  =  $1;  }
|  register_type { $$  =  $1; }
;

unit:
  S_LPAREN  S_RPAREN  {  $$  =  MKNODE(Unit);  }
;

natural_constant:
  NUMBER  {  $$  =  MKNODE(NaturalConstant,  $1);  }
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
;

application:
  term  term  {  $$  =  MKNODE(Application,  $1,  $2);  }
;

ascription:
  term  K_AS  type_name  {  $$  =  MKNODE(Ascription,  $1,  $3);  }
;

if_then_else:
  K_IF  term  K_THEN  term  K_ELSE  term  {  $$  =  MKNODE(Conditional,  $2,  $4,  $6);  }
;

pattern:
  IDENTIFIER { $$ = MKNODE(Identifier, $1); }
| tuple_pattern { $$ = $1; }
| register_pattern { $$ = $1; }
;

let_in:
  K_LET  pattern  S_EQ  term K_IN term  {  $$  =  MKNODE(LocalDefinition,  $2,  $4,  $6);  }
;

fix:
  K_FIX term  {  $$  =  MKNODE(Fix,  $2);  }
;

let_rec:
  K_LETREC IDENTIFIER S_COLON type_name S_EQ term K_IN term  {
    $$  =  MKNODE(LocalDefinition,  MKNODE(Identifier,  $2),  MKNODE(Fix,  MKNODE(Abstraction,  MKNODE(Variable,  $2,  $4),  $6)),  $8);
  }
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
  S_LBRACKET tuple_terms S_RBRACKET { $$ = MKNODE(Tuple, $2.begin(), $2.end()); }
;

tuple_type:
  S_LBRACKET tuple_type_types S_RBRACKET { $$ = MKTYPE(TupleType, $2.begin(), $2.end()); }
;

tuple_type_types:
  type_name { $$ = { $1 }; }
| type_name S_COMMA tuple_type_types  { $3.push_front($1); $$ = $3; }
;

tuple_terms:
  term                      { $$ = { $1 }; }
| term S_COMMA tuple_terms  { $3.push_front($1); $$ = $3; };
;

tuple_pattern:
  S_LBRACKET tuple_pattern_terms S_RBRACKET { $$ = MKNODE(Tuple, $2.begin(), $2.end()); }
;

tuple_pattern_terms:
  pattern { $$ = { $1 }; }
| pattern S_COMMA tuple_pattern_terms  { $3.push_front($1); $$ = $3; };
;

register:
  S_LBRACKET register_terms S_RBRACKET { $$ = MKNODE(Register, $2); }
;

register_terms:
  IDENTIFIER S_COLON term { $$ = { {$1, $3} }; }
| IDENTIFIER S_COLON term S_COMMA register_terms  { $5.insert({$1, $3}); $$ = $5; }
;

register_type:
  S_LBRACKET register_type_types S_RBRACKET { $$ = MKTYPE(RegisterType, $2); }
;

register_type_types:
  IDENTIFIER S_COLON type_name { $$ = { {$1, $3} }; }
| IDENTIFIER S_COLON type_name S_COMMA register_type_types  { $5.insert({$1, $3}); $$ = $5; }
;

register_pattern:
  S_LBRACKET register_pattern_terms S_RBRACKET { $$ = MKNODE(Register, $2); }
;

register_pattern_terms:
  IDENTIFIER S_COLON pattern { $$ = { {$1, $3} }; }
| IDENTIFIER S_COLON pattern S_COMMA register_pattern_terms  { $5.insert({$1, $3}); $$ = $5; }
;

operator_dot:
  term S_DOT IDENTIFIER { $$  =  MKNODE(OperatorDot,  $1,  MKNODE(Identifier,  $3)); }
| term S_DOT NUMBER  {  $$  =  MKNODE(OperatorDot,  $1,  MKNODE(NaturalConstant,  $3));  }
;

term:
  S_LPAREN  term  S_RPAREN  {  $$  =  $2;  }
|  unit  {  $$  =  $1;  }
|  natural_constant  {  $$  =  $1;  }
|  string_constant  {  $$  =  $1;  }
|  bool_constant  {  $$  =  $1;  }
|  abstraction  {  $$  =  $1;  }
|  ascription { $$ = $1; }
|  application  {  $$  =  $1;  }
|  identifier  {  $$  =  $1;  }
| if_then_else  { $$ = $1; }
| let_in  { $$ = $1; }
| sequence  { $$ = $1; }
| fix  { $$ = $1; }
| let_rec  { $$ = $1; }
| succ  { $$ = $1; }
| pred  { $$ = $1; }
| iszero  { $$ = $1; }
| print  { $$ = $1; }
| tuple  { $$ = $1; }
| register  { $$ = $1; }
| operator_dot { $$ = $1; }
;

%%

/**
  *  Implement  error  function
  */
void  yy::Parser::error(const  location_type  &l,  const  std::string  &message)
{
  throw  yy::Parser::syntax_error(l,  message);
}