%skeleton "lalr1.cc"
%require  "3.4"
%debug
%defines
%define api.parser.class {Parser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
//
// Headers used for token types must be included here
//
#include <sstream>
#include <string>
#include <ast/ast.hpp>

// Required for %param
namespace yy { class Driver; }
}

%param { yy::Driver  *driver }
%locations
%define parse.error verbose

%code {
// Driver include must be here (YY_DECL and YY_DRIVERDATA)
#include <ast_driver.hpp>

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
s:
      term { YY_DRIVERDATA->setRoot($1); }
    | error {};

term:
      IDENTIFIER  { $$ = MKNODE(Identifier, $1); }
    | K_LAMBDA IDENTIFIER S_DOT term { $$ = MKNODE(Abstraction, MKNODE(Identifier, $2), $4); }
    | S_LPAREN term S_RPAREN { $$ = $2; }
    | term S_SPACE term { $$ = MKNODE(Application, $1, $3); }
    | error {};

%%

/**
 * Implement error function
 */
void yy::Parser::error(const location_type &l, const std::string &message)
{
    auto os = std::ostringstream();
    os << l << ": " << message << std::endl;
    // TODO: Change exception type
    throw std::runtime_error(os.str());
}