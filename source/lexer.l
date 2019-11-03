%option noyywrap nounput noinput batch debug

%{
#include <parser.hpp>
#include <driver.hpp>

// Code run each time a pattern is matched
#define YY_USER_ACTION  loc.columns(yyleng);

static std::string strbuf;
%}

WHITESPACE  [ \t]

%x STRING

%%
%{
  // A handy shortcut to the location held by the driver
  // driver variable comes from YY_DECL prototype argument
  yy::location& loc = driver->yyLocation();
  // Code run each time yylex is called
  loc.step ();
%}

{WHITESPACE}+           { loc.step(); }
\r?\n+                  { loc.lines(yyleng); loc.step(); }

\"                                  { BEGIN STRING; strbuf.clear(); }
<STRING>[^\\"\n]*                   { strbuf += yytext; }
<STRING>\\n                         { strbuf += '\n'; }
<STRING>\\t                         { strbuf += '\t'; }
<STRING>\\x[0-9A-Fa-f][0-9A-Fa-f]?  { strbuf += strtol(yytext+2, 0, 16); }
<STRING>\\[\\"]                     { strbuf += yytext[1]; }
<STRING>\"                          {  BEGIN INITIAL; return yy::Parser::make_STRING(strbuf.data(), loc); }


\(                      { return yy::Parser::make_S_LPAREN(loc); }
\)                      { return yy::Parser::make_S_RPAREN(loc); }
\{                      { return yy::Parser::make_S_LBRACKET(loc); }
\}                      { return yy::Parser::make_S_RBRACKET(loc); }
\,                      { return yy::Parser::make_S_COMMA(loc); }
\.                      { return yy::Parser::make_S_DOT(loc); }
\=                      { return yy::Parser::make_S_EQ(loc); }
\:                      { return yy::Parser::make_S_COLON(loc); }
;;                      { return yy::Parser::make_S_LINE_END(loc); }
;                       { return yy::Parser::make_S_SEMICOLON(loc); }
->                      { return yy::Parser::make_S_ARROW(loc); }

print                   { return yy::Parser::make_K_PRINT(loc); }
succ                    { return yy::Parser::make_K_SUCC(loc); }
pred                    { return yy::Parser::make_K_PRED(loc); }
iszero                  { return yy::Parser::make_K_ISZERO(loc); }
true                    { return yy::Parser::make_K_TRUE(loc); }
false                   { return yy::Parser::make_K_FALSE(loc); }
if                      { return yy::Parser::make_K_IF(loc); }
then                    { return yy::Parser::make_K_THEN(loc); }
else                    { return yy::Parser::make_K_ELSE(loc); }
let                     { return yy::Parser::make_K_LET(loc); }
letrec                  { return yy::Parser::make_K_LETREC(loc); }
in                      { return yy::Parser::make_K_IN(loc); }
lambda                  { return yy::Parser::make_K_LAMBDA(loc); }
as                      { return yy::Parser::make_K_AS(loc); }
Dyn                     { return yy::Parser::make_K_DYN(loc); }


[0-9]+                  { return yy::Parser::make_NUMBER(atoi(yytext), loc); }
[a-z_][a-z0-9_]*        { return yy::Parser::make_IDENTIFIER(yytext, loc); }
[A-Z][a-zA-Z]*          { return yy::Parser::make_TYPE_NAME(yytext, loc); }
<<EOF>>                 { return yy::Parser::make_END(loc); }
.                       { throw std::runtime_error(std::string("error: ") + yytext); }

%%
