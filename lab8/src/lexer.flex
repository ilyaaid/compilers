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


IDENT [A-Z][A-Za-z]*\'*

%x COMMENT STRING

%%

{IDENT} {
   lval->tag = DOMAIN_TAG::TAG_IDENT;
   return lval->tag;
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
   // lval->tag = DOMAIN_TAG::TAG_COMMENT;
   // return lval->tag;
}

<COMMENT><<EOF>> {
   std::cout << "error in comment!" << std::endl;
   // lval->tag = DOMAIN_TAG::TAG_END;
   return driver.end(lval);
}

\" {
   BEGIN(STRING);
   driver.continued = true;
}

<STRING>\" {
   BEGIN(0);
   lval->tag = DOMAIN_TAG::TAG_STRING;
   return lval->tag;
}

<STRING>\n |

<STRING><<EOF>> {
   throw std::runtime_error("error in token STRING:\n\treceived:EOF\n\texpected:\"");
   // lval->tag = DOMAIN_TAG::TAG_END;
   // return lval->tag;
}

<STRING>. {
   driver.continued = true;
}

\( {
   lval->tag = DOMAIN_TAG::TAG_LPAREN;
   return lval->tag;
}

\) {
   lval->tag = DOMAIN_TAG::TAG_RPAREN;
   return lval->tag;
}

\* {
   lval->tag = DOMAIN_TAG::TAG_STAR;
   return lval->tag;
}

[ \t\r\n]+ {
   // lval->tag = DOMAIN_TAG::TAG_WHITESPACE;
   // return lval->tag;
}

. {
   throw std::runtime_error("error token:\n\treceived:"+lval->attr);
   // lval->tag = DOMAIN_TAG::TAG_ERROR;
   // return lval->tag;
}


%%
