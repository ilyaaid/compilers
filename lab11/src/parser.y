%skeleton "lalr1.cc"
%require  "3.0"

%define api.namespace { ps }
%define api.parser.class { Parser }


%code requires{
  class Driver;
  class Scanner;
}


%parse-param{ Scanner & scanner }
%parse-param{ Driver & driver }



%code{
   #include <iostream>
   #include <string>

  /* include for all driver functions */
#include "scanner.hpp"
#include "driver.hpp"

#undef yylex
#define yylex scanner.lex
}

%define api.value.type variant

%token END 0
%token <std::string>
TAG_NUMBER
TAG_BOOL_NUMBER
TAG_IDENT
TAG_IF
TAG_THEN
TAG_ELSE
TAG_OR
TAG_AND
TAG_NOT
TAG_TYPE
TAG_LPAREN
TAG_RPAREN
TAG_LQPAREN
TAG_RQPAREN
TAG_COLON
TAG_SEMICOLON
TAG_COMMA
TAG_EQUAL
TAG_MULT
TAG_DIV
TAG_PLUS
TAG_MINUS

%nterm inc dec
%nterm <std::string>
Program
FuncD
FuncDParams
Var
VarTail
VarDef
VarDefList
VarDefListTail

Expr
SimpleExpr
Exprs
ExprsEmpty
ExprsTail

IfStatement
ElseIfPart

ArithExpr
T1
T1Op
T1Tail
T2
T2Op
T2Tail
T3
T3Op

Type
TupleTypeList

NumberLit
BoolLit
ListLit
Parens




%%
%start Program;

Program:
FuncD Program{
  $$ = $1 + $2;
  driver.result = $$;
}
| %empty {}
;

FuncD:
TAG_IDENT FuncDParams TAG_COLON Type TAG_EQUAL inc Expr TAG_SEMICOLON dec{
  $$ = $1 + $2 + $3 + $4 + $5;
  $$ += '\n' + std::string(driver.enclosure + 1, ' ');
  $$ += $7 + $8;
  $$ += '\n' + std::string(driver.enclosure, ' ');
}
;

inc: %empty { driver.enclosure++; };
dec: %empty { driver.enclosure--; };

FuncDParams:
TAG_LPAREN VarDefList TAG_RPAREN{ $$ = $1 + $2 + $3; }
;

VarDefList:
VarDef VarDefListTail{ $$ = $1 + $2; }
| %empty{}
;

VarDefListTail:
TAG_COMMA VarDef VarDefListTail{ $$ = $1 + $2 + $3; }
| %empty{}
;

VarDef:
TAG_IDENT TAG_COLON Type{ $$ = $1 + $2 + $3; }
;


Type:
TAG_LQPAREN Type TAG_RQPAREN{ $$ = $1 + $2 + $3; }
| TAG_LPAREN Type TAG_COMMA Type TupleTypeList TAG_RPAREN{ $$ = $1 + $2 + $3 + $4 + $5 + $6; }
| TAG_TYPE{ $$ = $1; }
;

TupleTypeList:
TAG_COMMA Type TupleTypeList{ $$ = $1 + $2 + $3; }
| %empty{}
;

Expr:
IfStatement { $$ = $1; }
| SimpleExpr { $$ = $1; }
;

SimpleExpr:
ArithExpr { $$ = $1; }
;

Exprs:
Expr ExprsTail { $$ = $1 + $2;}
;

ExprsEmpty:
Exprs {$$ = $1; }
| %empty {}
;

ExprsTail:
TAG_COMMA Expr ExprsTail { $$ = $1 + $2 + $3; }
| %empty {}
;

IfStatement:
TAG_IF Expr TAG_THEN inc Expr dec TAG_ELSE ElseIfPart inc SimpleExpr dec {
  $$ = $1 + ' ' + $2 + ' ' + $3;
  $$ += '\n' + std::string(driver.enclosure + 1, ' ');
  $$ += $5;
  $$ += '\n' + std::string(driver.enclosure, ' ');
  $$ += $7 + $8;
  $$ += '\n' + std::string(driver.enclosure + 1, ' ');
  $$ += $10;
 }
;

ElseIfPart:
TAG_IF Expr TAG_THEN inc Expr dec TAG_ELSE ElseIfPart {
  $$ = ' ' + $1 + ' ' + $2 + ' ' + $3;
  $$ += '\n' + std::string(driver.enclosure + 1, ' ');
  $$ += $5;
  $$ += '\n' + std::string(driver.enclosure, ' ');
  $$ += $7 + $8;
}
| %empty {}
;


Var:
TAG_IDENT VarTail {$$ = $1 + $2; } 
;

VarTail:
TAG_LPAREN ExprsEmpty TAG_RPAREN {$$ = $1 + $2 + $3;}
| %empty {}
;

NumberLit:
TAG_NUMBER {$$ = $1;}
;

BoolLit:
TAG_BOOL_NUMBER {$$ = $1;}
;

ListLit:
TAG_LQPAREN ExprsEmpty TAG_RQPAREN { $$ = $1 + $2 + $3; }
;

Parens:
TAG_LPAREN Expr ExprsTail TAG_RPAREN { $$ = $1 + $2 + $3 + $4; }
;


/* ================ ArithExpr ===============*/

ArithExpr:
T1 T1Tail { $$ = $1 + $2; }
;

T1Tail:
T1Op T1 T1Tail { $$ = $1 + $2 + $3; }
| %empty {}
;

T1Op:
TAG_PLUS { $$ = $1; }
| TAG_MINUS {$$ = $1; }
| TAG_OR { $$ = $1; }
;


T1:
T2 T2Tail { $$ = $1 + $2; }
;

T2Tail:
T2Op T2 T2Tail { $$ = $1 + $2 + $3; }
| %empty {}
;

T2Op:
TAG_MULT { $$ = $1; }
| TAG_DIV {$$ = $1; }
| TAG_AND { $$ = $1; }
;

T2:
T3Op T3 {$$ = $1 + $2; }
;

T3Op:
TAG_MINUS { $$ = $1; }
| TAG_NOT { $$ = $1; }
| %empty {}
;

T3:
Parens {$$ = $1; }
| BoolLit {$$ = $1; }
| NumberLit {$$ = $1; }
| ListLit {$$ = $1; }
| Var {$$ = $1; }

%%


void ps::Parser::error(const std::string& err_message)
{
  std::cerr << "Error: " << err_message;
}
