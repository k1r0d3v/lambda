%option noyywrap nounput noinput batch debug

%{
#include <parser.hpp>
#include <driver.hpp>

// Code run each time a pattern is matched
#define YY_USER_ACTION  loc.columns(yyleng);
%}

WHITESPACE  [ \t]

%%
%{
  // A handy shortcut to the location held by the driver
  // driver variable comes from YY_DECL prototype argument
  yy::location& loc = driver->yyLocation();
  // Code run each time yylex is called
  loc.step ();
%}

{WHITESPACE}+           { loc.step(); }
\n+                     { loc.lines(yyleng); loc.step(); }

\(                      { return yy::Parser::make_S_LPAREN(loc); }
\)                      { return yy::Parser::make_S_RPAREN(loc); }
\.                      { return yy::Parser::make_S_DOT(loc); }
\=                      { return yy::Parser::make_S_EQ(loc); }
\;                      { return yy::Parser::make_S_SEMICOLON(loc); }

succ                    { return yy::Parser::make_K_SUCC(loc); }
pred                    { return yy::Parser::make_K_PRED(loc); }
iszero                  { return yy::Parser::make_K_ISZERO(loc); }
true                    { return yy::Parser::make_K_TRUE(loc); }
false                   { return yy::Parser::make_K_FALSE(loc); }
if                      { return yy::Parser::make_K_IF(loc); }
then                    { return yy::Parser::make_K_THEN(loc); }
else                    { return yy::Parser::make_K_ELSE(loc); }
let                     { return yy::Parser::make_K_LET(loc); }
in                      { return yy::Parser::make_K_IN(loc); }
lambda                  { return yy::Parser::make_K_LAMBDA(loc); }

[0-9]+                  { return yy::Parser::make_NUMBER(atoi(yytext), loc); }
[a-z][a-z0-9]*          { return yy::Parser::make_IDENTIFIER(yytext, loc); }
<<EOF>>                 { return yy::Parser::make_END(loc); }
.                       { throw std::runtime_error(std::string("error: ") + yytext); }

%%
