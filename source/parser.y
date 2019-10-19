%skeleton "lalr1.cc"
%require  "3.4"
%debug
%defines
%define api.namespace {lambda}
%define api.parser.class {Parser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
//
// Headers used for token types must be included here
//
#include <string>
#include <ast/ast.hpp>

// Required for %param
namespace lambda { class DriverBase; }
}

%param { lambda::DriverBase  *driver  }
%locations
%define parse.error verbose

%code {
// Definition of YY_DECL must be included here
#include <ast_driver.hpp>
//

#ifndef YYDRIVERDATA
#error "You must define a driver data accessor. For example: #define YYDRIVERDATA (driver->data)"
#endif

// Make the ast operations shorter
#define MKNODE(T, ...) ast::Node::make<ast::T>(__VA_ARGS__)
}


%token END 0                    "EOF"
%token <int> NUMBER             "number"
%token <std::string> IDENTIFIER "identifier"

%token K_LAMBDA                 "lambda"
%token S_DOT                    "."
%token S_SPACE                  "space"
%token S_LPAREN                 "( "
%token S_RPAREN                 ")"

%type <ast::Node::Reference> term
%type s

%%
%start s;

s:
      term { YYDRIVERDATA->setRoot($1); }
    | error {};

term:
      IDENTIFIER  { $$ = MKNODE(Identifier, $1); }
    | K_LAMBDA IDENTIFIER S_DOT term { $$ = MKNODE(Abstraction, MKNODE(Identifier, $2), $4); }
    | S_LPAREN term S_RPAREN { $$ = $2; }
    | term S_SPACE term { $$ = MKNODE(Application, $1, $3); }
    | error {};

%%
