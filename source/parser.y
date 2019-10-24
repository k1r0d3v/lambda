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

%token K_SUCC                   "succ"
%token K_PRED                   "pred"
%token K_ISZERO                 "iszero"
%token K_TRUE                   "true"
%token K_FALSE                  "false"
%token K_IF                     "if"
%token K_THEN                   "then"
%token K_ELSE                   "else"
%token K_LET                    "let"
%token K_IN                     "in"
%token K_LAMBDA                 "lambda"

%token S_LPAREN                 "("
%token S_RPAREN                 ")"
%token S_DOT                    "."
%token S_EQ                     "="
%token S_SEMICOLON              ";"



%type <ast::Node::Pointer> term
%type <ast::AST*> s

%%
s:
      term { YY_DRIVERDATA->setRoot($1); $$ = YY_DRIVERDATA; }
    | term S_SEMICOLON s { YY_DRIVERDATA->setRoot(ast::Sequence::join($1, YY_DRIVERDATA->root())); $$ = YY_DRIVERDATA; }
    | END { $$ = YY_DRIVERDATA; }
    | error { };

term: S_LPAREN term S_RPAREN { $$ = $2; }
    | S_LPAREN S_RPAREN { $$ = MKNODE(Unit); }
    | NUMBER { $$ = MKNODE(Natural, $1); }
    | IDENTIFIER  { $$ = MKNODE(Identifier, $1); }
    | K_TRUE { $$ = MKNODE(Boolean, true); }
    | K_FALSE { $$ = MKNODE(Boolean, false); }
    | K_LET IDENTIFIER S_EQ term K_IN term { $$ = MKNODE(LocalDefinition, MKNODE(Identifier, $2), $4, $6); }
    | K_IF term K_THEN term K_ELSE term { $$ = MKNODE(Conditional, $2, $4, $6); }
    | K_LAMBDA IDENTIFIER S_DOT term { $$ = MKNODE(Abstraction, MKNODE(Identifier, $2), $4); }
    | K_SUCC term {  $$ = MKNODE(Successor, $2); }
    | K_PRED term { $$ = MKNODE(Predecessor, $2); }
    | K_ISZERO term { $$ = MKNODE(IsZero, $2); }
    | term term { $$ = MKNODE(Application, $1, $2); }
    | error { };

%%

/**
 * Implement error function
 */
void yy::Parser::error(const location_type &l, const std::string &message)
{
    auto os = std::ostringstream();
    os << l << ": " << message;
    // TODO: Change exception type
    throw std::runtime_error(os.str());
}