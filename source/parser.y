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
#include <deque>
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
#define MKTYPE(T, ...) ast::Type::make<ast::T>(__VA_ARGS__)
}


%token END 0                    "EOF"
%token <int> NUMBER             "number"
%token <std::string> IDENTIFIER "identifier"
%token <std::string> TYPE_NAME  "type name"
%token <std::string> STRING     "string"

%token K_PRINT                  "print"
%token K_SUCC                   "succ"
%token K_PRED                   "pred"
%token K_ISZERO                 "iszero"
%token K_TRUE                   "true"
%token K_FALSE                  "false"
%token K_IF                     "if"
%token K_THEN                   "then"
%token K_ELSE                   "else"
%token K_LET                    "let"
%token K_LETREC                 "letrec"
%token K_FIX                    "fix"
%token K_IN                     "in"
%token K_LAMBDA                 "lambda"
%token K_AS                     "as"
%token K_DYN                    "Dyn"
%token K_BOOL                   "Bool"
%token K_NAT                    "Nat"
%token K_STR                    "Str"
%token K_UNIT                   "Unit"

%token S_LPAREN                 "("
%token S_RPAREN                 ")"
%token S_DOT                    "."
%token S_COMMA                  ","
%token S_EQ                     "="
%token S_COLON                  ":"
%token S_SEMICOLON              ";"
%token S_LINE_END               ";;"
%token S_ARROW                  "->"
%token S_LBRACKET               "{"
%token S_RBRACKET               "}"


%type <ast::Node::Pointer> term
%type <ast::Type::Pointer> variable_type
%type <std::deque<ast::Type::Pointer>> tuple_types
%type <std::deque<ast::Node::Pointer>> tuple_terms
%type <std::deque<ast::Pattern::Pointer>> tuple_patterns
%type <std::map<ast::string, ast::Type::Pointer>> register_types
%type <std::map<ast::string, ast::Node::Pointer>> register_terms
%type <std::map<ast::string, ast::Pattern::Pointer>> register_patterns
%type <ast::Pattern::Pointer> pattern
%type <ast::Node::Pointer> sequence
%type <ast::AST*> s

%precedence K_IN
%precedence S_LINE_END
%precedence S_SEMICOLON

%%
/* TODO: Clean the grammar please!! */

/**
 *  Start
 */

s:    sequence S_LINE_END s { YY_DRIVERDATA->setRoot(ast::Sequence::join($1, YY_DRIVERDATA->root())); $$ = YY_DRIVERDATA; }
    | K_LET IDENTIFIER S_EQ sequence S_LINE_END s { YY_DRIVERDATA->setRoot(ast::Sequence::join(MKNODE(Declaration, MKNODE(Identifier, $2), $4), YY_DRIVERDATA->root())); $$ = YY_DRIVERDATA; }
    | END { $$ = YY_DRIVERDATA; }
    | error { };

/**
 *  Types
 */

variable_type: S_LPAREN variable_type S_RPAREN { $$ = $2; }
    | TYPE_NAME { $$ = MKTYPE(ConstantType, ast::TypeKind::Composed, $1); }
    | K_BOOL { $$ = MKTYPE(BoolType); }
    | K_NAT { $$ = MKTYPE(NatType); }
    | K_STR { $$ = MKTYPE(StrType); }
    | K_UNIT { $$ = MKTYPE(UnitType); }
    | K_DYN { $$ = MKTYPE(DynType); }
    | variable_type S_ARROW variable_type { $$ = MKTYPE(ArrowType, $1, $3); }
    | S_LBRACKET tuple_types S_RBRACKET { $$ = MKTYPE(TupleType, ast::list<ast::Type::Pointer>({$2.begin(), $2.end()})); }
    | S_LBRACKET register_types S_RBRACKET { $$ = MKTYPE(RegisterType, $2); }
    | error { };

tuple_types: variable_type { $$ = { $1 }; }
    | variable_type S_COMMA tuple_types  { $3.push_front($1); $$ = $3; };

register_types: IDENTIFIER S_COLON variable_type { $$ = { {$1, $3} }; }
    | IDENTIFIER S_COLON variable_type S_COMMA register_types  { $5.insert({$1, $3}); $$ = $5; };

/**
 *  Patterns
 */

pattern: IDENTIFIER { $$ = MKNODE(Identifier, $1); }
    | S_LBRACKET tuple_patterns S_RBRACKET { $$ = MKNODE(Tuple, ast::list<ast::Node::Pointer>({$2.begin(), $2.end()})); }
    | S_LBRACKET register_patterns S_RBRACKET { $$ = MKNODE(Register, $2); }
    | error { };

tuple_patterns: pattern { $$ = { $1 }; }
     | pattern S_COMMA tuple_patterns  { $3.push_front($1); $$ = $3; };

register_patterns: IDENTIFIER S_COLON pattern { $$ = { {$1, $3} }; }
     | IDENTIFIER S_COLON pattern S_COMMA register_patterns  { $5.insert({$1, $3}); $$ = $5; };

/**
 *  Terms
 */

term: S_LPAREN sequence S_RPAREN { $$ = $2; }
    | S_LPAREN S_RPAREN { $$ = MKNODE(Unit); }
    | NUMBER { $$ = MKNODE(NaturalConstant, $1); }
    | IDENTIFIER  { $$ = MKNODE(Identifier, $1); }
    | STRING  { $$ = MKNODE(StringConstant, $1); }
    | K_TRUE { $$ = MKNODE(BooleanConstant, true); }
    | K_FALSE { $$ = MKNODE(BooleanConstant, false); }
    | K_LET pattern S_EQ sequence K_IN sequence { $$ = MKNODE(LocalDefinition, $2, $4, $6); }
    | K_IF term K_THEN term K_ELSE term { $$ = MKNODE(Conditional, $2, $4, $6); }
    | K_LAMBDA IDENTIFIER S_COLON variable_type S_DOT sequence { $$ = MKNODE(Abstraction, MKNODE(Variable, $2, $4), $6); }
    | K_SUCC term {  $$ = MKNODE(Successor, $2); }
    | K_PRED term { $$ = MKNODE(Predecessor, $2); }
    | K_ISZERO term { $$ = MKNODE(IsZero, $2); }
    | K_PRINT term {  $$ = MKNODE(Print, $2); }
    | term term { $$ = MKNODE(Application, $1, $2); }
    | term K_AS variable_type { $$ = MKNODE(Ascription, $1, $3); }
    | S_LBRACKET tuple_terms S_RBRACKET { $$ = MKNODE(Tuple, ast::list<ast::Node::Pointer>({$2.begin(), $2.end()})); }
    | S_LBRACKET register_terms S_RBRACKET { $$ = MKNODE(Register, $2); }
    | term S_DOT IDENTIFIER { $$ = MKNODE(OperatorDot, $1, MKNODE(Identifier, $3)); }
    | term S_DOT NUMBER { $$ = MKNODE(OperatorDot, $1, MKNODE(NaturalConstant, $3)); }
    | K_FIX term { $$ = MKNODE(Fix, $2); }
    | K_LETREC IDENTIFIER S_COLON variable_type S_EQ term K_IN term { $$ = MKNODE(LocalDefinition, MKNODE(Identifier, $2), MKNODE(Fix, MKNODE(Abstraction, MKNODE(Variable, $2, $4), $6)), $8); }
    | error { };

tuple_terms: term { $$ = { $1 }; }
    | term S_COMMA tuple_terms  { $3.push_front($1); $$ = $3; };

register_terms: IDENTIFIER S_COLON term { $$ = { {$1, $3} }; }
    | IDENTIFIER S_COLON term S_COMMA register_terms  { $5.insert({$1, $3}); $$ = $5; };

sequence: term { $$ = $1; }
        | term S_SEMICOLON sequence { $$ = ast::Sequence::join($1, $3); };

%%

/**
 * Implement error function
 */
void yy::Parser::error(const location_type &l, const std::string &message)
{
    throw yy::Parser::syntax_error(l, message);
}