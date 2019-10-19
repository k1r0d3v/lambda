%option noyywrap nounput noinput batch debug

%{
#include <lambda/parser.hpp>
#include <driver.hpp>

// Code run each time a pattern is matched
# define YY_USER_ACTION  loc.columns (yyleng);
%}

WHITESPACE  [ \t]

%%
%{
  // A handy shortcut to the location held by the driver
  // driver variable comes from YY_DECL prototype argument
  lambda::location& loc = driver->location();
  // Code run each time yylex is called
  loc.step ();
%}

{WHITESPACE}+           { loc.step(); return lambda::Parser::make_S_SPACE(loc); }
\n+                     { loc.lines(yyleng); loc.step(); }

\({WHITESPACE}*         { return lambda::Parser::make_S_LPAREN(loc); }
\)                      { return lambda::Parser::make_S_RPAREN(loc); }
\.{WHITESPACE}*         { return lambda::Parser::make_S_DOT(loc); }
lambda{WHITESPACE}+     { return lambda::Parser::make_K_LAMBDA(loc); }
[0-9]+                  { return lambda::Parser::make_NUMBER(atoi(yytext), loc); }
[a-z][a-z0-9]*          { return lambda::Parser::make_IDENTIFIER(yytext, loc); }
<<EOF>>                 { return lambda::Parser::make_END(loc); }
.                       {/* TODO: Throw error */}

%%
