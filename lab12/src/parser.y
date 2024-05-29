%skeleton "lalr1.cc"
%require  "3.0"

%define api.namespace { ps }
%define api.parser.class { Parser }
%define parse.assert

%code requires{
  #include "ast_tree.hpp"
  #include <memory>

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

%token < int >
TAG_NUMBER
;

%token < bool >
TAG_BOOL_NUMBER
;

%token < SimpleType::TYPE >
TAG_TYPE
;

%token <std::string>
TAG_IDENT
TAG_IF
TAG_THEN
TAG_ELSE
TAG_OR
TAG_AND
TAG_NOT
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

/*================== NON TERMS ==============*/

%nterm <std::string>
T1Op
T2Op
T3Op
;


%nterm < std::vector<std::shared_ptr<Function>> >
ProgramTail
;

%nterm < std::shared_ptr<Function> >
FuncD
;

%nterm < std::vector<std::shared_ptr<VarDef>> >
FuncDParams
VarDefList
VarDefListTail
;

%nterm < std::shared_ptr<Type> >
Type
;

%nterm < std::vector<std::shared_ptr<Type>> > 
TupleTypeList
;

%nterm < std::shared_ptr<VarDef> >
VarDef
;

%nterm < std::shared_ptr<Expr> >
Expr
SimpleExpr
ArithExpr
T1
T2
T3

Var
NumberLit
BoolLit
ListLit
Parens

IfExpr
;

%nterm < std::vector<std::shared_ptr<Expr>> >
Exprs
ExprsEmpty
ExprsTail
;

%nterm < std::pair<bool, std::vector<std::shared_ptr<Expr>>> >
VarTail
;

%nterm < std::pair<std::string, std::shared_ptr<Expr>> >
T1Tail
T2Tail
;

/*===================== Rules ======================*/

%locations

%%
%start Program;

Program:
FuncD ProgramTail {
  auto vec = std::move($2);
  vec.insert(vec.begin(), std::move($1));
  driver.program = std::make_shared<Program>(std::move(vec), @1);
}
;

ProgramTail:
FuncD ProgramTail {
  $$ = std::move($2);
  $$.insert($$.begin(), std::move($1));
}
| %empty {
  $$ = std::vector<std::shared_ptr<Function>>();
}
;

FuncD:
TAG_IDENT FuncDParams TAG_COLON Type TAG_EQUAL Expr TAG_SEMICOLON {
  $$ = std::make_shared<Function>($1, std::move($2), std::move($4), std::move($6), @1);
}
;

FuncDParams:
TAG_LPAREN VarDefList TAG_RPAREN{
  $$ = std::move($2);
}
;

VarDefList:
VarDef VarDefListTail{ 
  $$ = std::move($2);
  $$.insert($$.begin(), std::move($1));
}
| %empty{
  $$ = std::vector<std::shared_ptr<VarDef>>();
}
;

VarDefListTail:
TAG_COMMA VarDef VarDefListTail{ 
  $$ = std::move($3);
  $$.insert($$.begin(), std::move($2));
}
| %empty{
  $$ = std::vector<std::shared_ptr<VarDef>>();
}
;

VarDef:
TAG_IDENT TAG_COLON Type{ 
  $$ = std::make_shared<VarDef>($1, std::move($3), @1);
}
;

Type:
TAG_LQPAREN Type TAG_RQPAREN{ 
  $$ = std::make_shared<ListType>(std::move($2));
}
| TAG_LPAREN Type TAG_COMMA Type TupleTypeList TAG_RPAREN{ 
  auto vec = std::move($5);
  vec.insert(vec.begin(), std::move($4));
  vec.insert(vec.begin(), std::move($2));
  $$ = std::make_shared<TupleType>(std::move(vec));
}
| TAG_TYPE {
  $$ = std::make_shared<SimpleType>($1);
}
;

TupleTypeList:
TAG_COMMA Type TupleTypeList{
  $$ = std::move($3);
  $$.insert($$.begin(), std::move($2));
}
| %empty{
  $$ = std::vector<std::shared_ptr<Type>>();
}
;

Expr:
IfExpr {
  $$ = std::move($1);
}
| SimpleExpr { 
  $$ = std::move($1);
}
;

SimpleExpr:
ArithExpr { 
  $$ = std::move($1);
}
;

Exprs:
Expr ExprsTail { 
  $$ = std::move($2);
  $$.insert($$.begin(), std::move($1));
}
;

ExprsEmpty:
Exprs { 
  $$ = std::move($1);
}
| %empty {
  $$ = std::vector<std::shared_ptr<Expr>>();
}
;

ExprsTail:
TAG_COMMA Expr ExprsTail { 
  $$ = std::move($3);
  $$.insert($$.begin(), std::move($2));
}
| %empty {
  $$ = std::vector<std::shared_ptr<Expr>>();
}
;

IfExpr:
TAG_IF Expr TAG_THEN Expr TAG_ELSE Expr {
  $$ = std::make_shared<IfExpr>(std::move($2), std::move($4), std::move($6), @1);
}
;


Var:
TAG_IDENT VarTail {
  auto pair = std::move($2);
  if (!pair.first) {
    $$ = std::make_shared<VariableLit>($1, @1);
  } else {
    $$ = std::make_shared<CallFunction>($1, std::move(pair.second), @1);
  }
} 
;

VarTail:
TAG_LPAREN ExprsEmpty TAG_RPAREN {
  $$ = std::make_pair(true, std::move($2));
}
| %empty {
  $$ = std::make_pair(false, std::vector<std::shared_ptr<Expr>>());
}
;

NumberLit:
TAG_NUMBER {
  $$ = std::make_shared<NumberLit>($1, @1);
}
;

BoolLit:
TAG_BOOL_NUMBER {
  $$ = std::make_shared<BoolNumberLit>($1, @1);
}
;

ListLit:
TAG_LQPAREN ExprsEmpty TAG_RQPAREN { 
  $$ = std::make_shared<ListLit>(std::move($2), @1);
}
;

Parens:
TAG_LPAREN Expr ExprsTail TAG_RPAREN { 
  auto vec = std::move($3);
  if (vec.empty()) {
    $$ = std::move($2);
  } else {
    vec.insert(vec.begin(), std::move($2));
    $$ = std::make_shared<TupleLit>(std::move(vec), @1);
  }
}
;


/* ================ ArithExpr ===============*/

ArithExpr:
T1 T1Tail { 
  if ($2.second) {
    $$ = std::make_shared<BinOpExpr>(std::move($1), $2.first, std::move($2.second), @1);
  } else {
    $$ = std::move($1);
  }
}
;

T1Tail:
T1Op T1 T1Tail { 
  std::shared_ptr<Expr> new_expr;
  if ($3.second) {
    new_expr = std::make_shared<BinOpExpr>(std::move($2), $3.first, std::move($3.second), @1);
  } else {
    new_expr = std::move($2);
  }
  $$ = std::make_pair($1, std::move(new_expr));
}
| %empty {
  $$ = {"", nullptr};
}
;

T1Op:
TAG_PLUS { $$ = $1; }
| TAG_MINUS { $$ = $1; }
| TAG_OR { $$ = $1; }
;


T1:
T2 T2Tail { 
  if ($2.second) {
    $$ = std::make_shared<BinOpExpr>(std::move($1), $2.first, std::move($2.second), @1);
  } else {
    $$ = std::move($1);
  }
}
;

T2Tail:
T2Op T2 T2Tail { 
  std::shared_ptr<Expr> new_expr;
  if ($3.second) {
    new_expr = std::make_shared<BinOpExpr>(std::move($2), $3.first, std::move($3.second), @1);
  } else {
    new_expr = std::move($2);
  }
  $$ = std::make_pair($1, std::move(new_expr));
}
| %empty {
  $$ = {"", nullptr};
}
;

T2Op:
TAG_MULT { $$ = $1; }
| TAG_DIV { $$ = $1; }
| TAG_AND { $$ = $1; }
;

T2:
T3Op T3 { 
  if (!$1.empty()) {
    $$ = std::make_shared<UnaryOpExpr>($1, std::move($2), @1);
  } else {
    $$ = std::move($2);
  }
}
;

T3Op:
TAG_MINUS { $$ = $1; }
| TAG_NOT { $$ = $1; }
| %empty { $$ = ""; }
;

T3:
Parens { $$ = std::move($1); }
| BoolLit { $$ = std::move($1); }
| NumberLit { $$ = std::move($1); } 
| ListLit { $$ = std::move($1); }
| Var { $$ = std::move($1); }

%%


void ps::Parser::error(const location_type& lloc, const std::string& err_message)
{
  std::cerr << "Error: " << err_message << " from: " << lloc.begin << " to: " << lloc.end;
}
