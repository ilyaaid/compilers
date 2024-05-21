%{
/* C++ string header, for string ops below */
#include <string>
#include <iostream>
#include <memory>

#include "scanner.hpp"

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


IDENT [A-Z][A-Za-z]*\'*

%x COMMENT STRING

%%

{IDENT} {
   std::replace(lval->begin(), lval->end(), '\'', '_');
   return 'i';
   // return DOMAIN_TAG::TAG_IDENT;
}

\/\* {
   BEGIN(COMMENT);
   driver.continued = true;
}

<COMMENT>. {
   driver.continued = true;
}

<COMMENT>\*\/ {
   BEGIN(INITIAL);
}

<COMMENT><<EOF>> {
   return driver.end(lval);
}

\" {
   BEGIN(STRING);
   driver.continued = true;
}

<STRING>\" {
   BEGIN(0);
   lval->erase(lval->begin());
   lval->erase(lval->begin() + lval->length() - 1);
   return 's';
   // return DOMAIN_TAG::TAG_STRING;
}

<STRING>\n |

<STRING><<EOF>> {
   throw std::runtime_error("error in token STRING:\n\treceived:EOF\n\texpected:\"");
}

<STRING>. {
   driver.continued = true;
}

\( {
   return '(';
   // return DOMAIN_TAG::TAG_LPAREN;
}

\) {
   return ')';
   // return DOMAIN_TAG::TAG_RPAREN;
}

\* {
   return '*';
   // return DOMAIN_TAG::TAG_STAR;
}

[ \t\r\n]+ {
}

. {
   throw std::runtime_error("error token:\n\treceived:"+*lval);
}


%%
