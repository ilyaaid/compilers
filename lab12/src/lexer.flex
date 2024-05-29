%{
/* C++ string header, for string ops below */
#include <string>
#include <iostream>
#include <memory>
#include <algorithm>

#include "scanner.hpp"
#include "driver.hpp"

/* Implementation of yyFlexScanner */ 
#undef  YY_DECL
#define YY_DECL \
 ps::Parser::token_type Scanner::lex(ps::Parser::value_type* lval, ps::Parser::location_type* lloc)

// using token = ps::Parser::token;
using DOMAIN_TAG = ps::Parser::token;

#define yyterminate() return (DOMAIN_TAG::END)

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

COMMENT \-\-[^\n]*\n

%%

{NUMBER} {
   lval->build< int >(std::stoi(yytext));
   return DOMAIN_TAG::TAG_NUMBER;
}

{BOOL_NUMBER}/{NON_IDENT} {
   lval->build< bool >(std::string(yytext) == "true" ? true : false);
   return DOMAIN_TAG::TAG_BOOL_NUMBER;
}

if/{NON_IDENT} {
   lval->build< std::string >(yytext);
   return DOMAIN_TAG::TAG_IF;
}

then/{NON_IDENT} {
   lval->build< std::string >(yytext);
   return DOMAIN_TAG::TAG_THEN;
}

else/{NON_IDENT} {
   lval->build< std::string >(yytext);
   return DOMAIN_TAG::TAG_ELSE;
}

or/{NON_IDENT} {
   lval->build< std::string >(yytext);
   return DOMAIN_TAG::TAG_OR;
}

and/{NON_IDENT} {
   lval->build< std::string >(yytext);
   return DOMAIN_TAG::TAG_AND;
}

not/{NON_IDENT} {
   lval->build< std::string >(yytext);
   return DOMAIN_TAG::TAG_NOT;
}

{TYPE}/{NON_IDENT} {
   lval->build< SimpleType::TYPE >(std::string(yytext) == "int" ? 
   SimpleType::TYPE::INT : 
   SimpleType::TYPE::BOOL);
   return DOMAIN_TAG::TAG_TYPE;
}

{IDENT} {
   lval->build< std::string >(yytext);
   return DOMAIN_TAG::TAG_IDENT;
}

{COMMENT} {
   lloc->lines();
}


\( {
   lval->build< std::string >(yytext);
   return DOMAIN_TAG::TAG_LPAREN;
}

\) {
   lval->build< std::string >(yytext);
   return DOMAIN_TAG::TAG_RPAREN;
}

\[ {
   lval->build< std::string >(yytext);
   return DOMAIN_TAG::TAG_LQPAREN;
}

\] {
   lval->build< std::string >(yytext);
   return DOMAIN_TAG::TAG_RQPAREN;
}

\, {
   lval->build< std::string >(yytext);
   return DOMAIN_TAG::TAG_COMMA;
}

\: {
   lval->build< std::string >(yytext);
   return DOMAIN_TAG::TAG_COLON;
}

\; {
   lval->build< std::string >(yytext);
   return DOMAIN_TAG::TAG_SEMICOLON;
}


\= {
   lval->build< std::string >(yytext);
   return DOMAIN_TAG::TAG_EQUAL;
}

\* {
   lval->build< std::string >(yytext);
   return DOMAIN_TAG::TAG_MULT;
}
\/ {
   lval->build< std::string >(yytext);
   return DOMAIN_TAG::TAG_DIV;
}
\+ {
   lval->build< std::string >(yytext);
   return DOMAIN_TAG::TAG_PLUS;
}
\- {
   lval->build< std::string >(yytext);
   return DOMAIN_TAG::TAG_MINUS;
}

\n {
   lloc->lines();
}

[ \t\r] {

}

. {
   throw std::runtime_error("error token!");
}

%%
