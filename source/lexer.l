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

{WHITESPACE}+           { loc.step(); return yy::Parser::make_S_SPACE(loc); }
\n+                     { loc.lines(yyleng); loc.step(); }

\({WHITESPACE}*         { return yy::Parser::make_S_LPAREN(loc); }
\)                      { return yy::Parser::make_S_RPAREN(loc); }
\.{WHITESPACE}*         { return yy::Parser::make_S_DOT(loc); }
lambda{WHITESPACE}+     { return yy::Parser::make_K_LAMBDA(loc); }
[0-9]+                  { return yy::Parser::make_NUMBER(atoi(yytext), loc); }
[a-z][a-z0-9]*          { return yy::Parser::make_IDENTIFIER(yytext, loc); }
<<EOF>>                 { return yy::Parser::make_END(loc); }
.                       {/* TODO: Throw error */}

%%
