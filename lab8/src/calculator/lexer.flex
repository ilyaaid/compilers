%{
/* C++ string header, for string ops below */
#include <string>
#include <iostream>
#include <memory>

#include "analyzer/scanner.hpp"

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


NUMBER [1-9][0-9]*

%%

{NUMBER} {
   return 'n';
}

\+ {
   return '+';
}

\* {
   return '*';
}

\( {
   return '(';
}

\) {
   return ')';
}

[ \t\r\n]+ {
}

. {
   throw std::runtime_error("error token:\n\treceived:"+*lval);
}


%%
