%{
/* C++ string header, for string ops below */
#include <string>
#include <iostream>
#include <memory>

#include "scanner.hpp"
#include "token.hpp"

/* Implementation of yyFlexScanner */ 
#undef  YY_DECL
#define YY_DECL \
 return_type Scanner::lex(std::shared_ptr<semantic_type>& lval, std::shared_ptr<location_type>& lloc)

#define yyterminate() return (driver.end(lval))

Driver driver;

#define YY_USER_ACTION \
{ \
   driver.common(lval, lloc, yytext); \
}

%}

%option debug
%option nodefault
%option yyclass="Scanner"
%option noyywrap
%option c++

NUMBER [0-9]*
BOOL_NUMBER true|false
IDENT \_*[A-Za-z][\_A-Za-z0-9]*
TYPE int|bool

NON_IDENT [^a-zA-Z\_0-9]

%x COMMENT

%%

{NUMBER} {
   return DOMAIN_TAG::TAG_NUMBER;
}

{BOOL_NUMBER}/{NON_IDENT} {
   return DOMAIN_TAG::TAG_BOOL_NUMBER;
}

if/{NON_IDENT} {
   return DOMAIN_TAG::TAG_IF;
}

then/{NON_IDENT} {
   return DOMAIN_TAG::TAG_THEN;
}

else/{NON_IDENT} {
   return DOMAIN_TAG::TAG_ELSE;
}

or/{NON_IDENT} {
   return DOMAIN_TAG::TAG_OR;
}

and/{NON_IDENT} {
   return DOMAIN_TAG::TAG_AND;
}

not/{NON_IDENT} {
   return DOMAIN_TAG::TAG_NOT;
}

{TYPE}/{NON_IDENT} {
   return DOMAIN_TAG::TAG_TYPE;
}

{IDENT} {
   return DOMAIN_TAG::TAG_IDENT;
}

\-\- {
   BEGIN(COMMENT);
   driver.continued = true;
}

<COMMENT>. {
   driver.continued = true;
}

<COMMENT>\r?\n {
   BEGIN(INITIAL);
}

<COMMENT><<EOF>> {
   return driver.end(lval);
}

\( {
   return DOMAIN_TAG::TAG_LPAREN;
}

\) {
   return DOMAIN_TAG::TAG_RPAREN;
}

\[ {
   return DOMAIN_TAG::TAG_LQPAREN;
}

\] {
   return DOMAIN_TAG::TAG_RQPAREN;
}

\, {
   return DOMAIN_TAG::TAG_COMMA;
}

\: {
   return DOMAIN_TAG::TAG_COLON;
}

\; {
   return DOMAIN_TAG::TAG_SEMICOLON;
}

\= {
   return DOMAIN_TAG::TAG_EQUAL;
}

\* {
   return DOMAIN_TAG::TAG_MULT;
}
\/ {
   return DOMAIN_TAG::TAG_DIV;
}
\+ {

   return DOMAIN_TAG::TAG_PLUS;
}
\- {
   
   return DOMAIN_TAG::TAG_MINUS;
}

{TYPE} {

   return DOMAIN_TAG::TAG_TYPE;
}

[ \t\r\n]+ {
}

. {
   throw std::runtime_error("error token:\n\treceived:" + *lval);
}


%%
