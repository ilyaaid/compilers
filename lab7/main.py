import parser.parser_edsl as pe
import re

from classes import *
# Добавление терминалов и нетерминалов ==================

NUMBER = pe.Terminal('INTEGER', '[0-9]+', int, priority=7)
IDENT = pe.Terminal('IDENT', '[A-Za-z][A-Za-z0-9_]*', str)

def make_keyword(image):
    return pe.Terminal(image, image, lambda name: None, priority=10)

KW_INT, KW_BOOL = map(make_keyword, 'int bool'.split())

KW_IF, KW_THEN, KW_ELSE, KW_OR, KW_AND, KW_PLUS, KW_MINUS, KW_MUL, KW_DIV = \
    map(make_keyword, 'if then else or and \+ \- \* \/'.split())


NProgram, NFuncD, NFuncDParams = \
    map(pe.NonTerminal, 'Program FuncD FuncDParams'.split())

NVarDef, NVarDefList, NVarDefListTail = \
    map(pe.NonTerminal, 'VarDef VarDefList VarDefListTail'.split())

NStatement, NIfStatement, NLogicExpr, NLogicExprTail, NLogicT1, NLogicT1Tail, NLogicT2, NLogicBrBlock, NLogicMulOp, NLogicAddOp = \
    map(pe.NonTerminal, 'Statement IfStatement LogicExpr LogicExprTail LogicT1 LogicT1Tail LogicT2 LogicBrBlock LogicMulOp LogicAddOp'.split())

NExpr, NExprList, NExprListTail = \
    map(pe.NonTerminal, 'Expr ExprList ExprListTail'.split())

NFuncC, NListLit, NTupleLit = \
    map(pe.NonTerminal, 'FuncC ListLit TupleLit'.split())

NArithExpr, NArithExprTail, NT1, NT1Tail, NT2, NBrBlock, NAddOp, NMulOp = \
    map(pe.NonTerminal, 'ArithExpr ArithExprTail T1 T1Tail T2 BrBlock AddOp MulOp'.split())

NType, NListType, NTupleType, NTupleTypeList = \
    map(pe.NonTerminal, 'Type ListType TupleType TupleTypeList'.split())

# Grammar ===============================

NProgram |= NFuncD, NProgram, lambda funcD, p: [funcD] + p
NProgram |= lambda: []

NFuncD  |= IDENT, NFuncDParams, ':', NType, '=', NExpr, ';', FuncDef
NFuncDParams |= '(', NVarDefList, ')', lambda vds: vds

NVarDef |= IDENT, ':', NType, VarDef
NVarDefList |= NVarDef, NVarDefListTail, lambda vd, vds: [vd] + vds
NVarDefList |= lambda: []
NVarDefListTail |= ',', NVarDef, NVarDefListTail, lambda vd, vds: [vd] + vds
NVarDefListTail |= lambda: []

NStatement |= NIfStatement
NIfStatement |= 'if', NLogicExpr, 'then', NExpr, 'else', NExpr, IfStatement

# LogicExpr ============================

def func1(term, res):
    if not res:
        return term
    return BinOpExpr(term, res['op'].name, res['expr'])

def func2(op, term, res):
    if not res:
        expr = term
    else:
        expr = BinOpExpr(term, res['op'].name, res['expr'])
    return {
        'op': op,
        'expr': expr
    }

NLogicExpr |= NLogicT1, NLogicExprTail, func1
NLogicExprTail |= NLogicAddOp, NLogicT1, NLogicExprTail, func2
NLogicExprTail |= lambda: {}
NLogicT1 |= NLogicT2, NLogicT1Tail, func1
NLogicT1Tail |= NLogicMulOp, NLogicT2, NLogicT1Tail, func2
NLogicT1Tail |= lambda: {}
NLogicAddOp |= KW_OR, lambda: KW_OR
NLogicMulOp |= KW_AND, lambda: KW_AND
NLogicT2 |= NExpr

# Expr ===============================

NExpr |= NStatement
NExpr |= NArithExpr
NExpr |= NListLit, ListLit
NExpr |= NTupleLit, TupleLit


NExprList |= NExpr, NExprListTail, lambda e, es : [e] + es
NExprList |= lambda: []
NExprListTail |= ',', NExpr, NExprListTail, lambda e, es: [e] + es
NExprListTail |= lambda: []

NFuncC |= IDENT, '(', NExprList, ')', FuncC

NListLit |= '[', NExprList, ']'
NTupleLit |= '(', NExpr, ',', NExpr, NExprListTail, ')', lambda e1, e2, es: [e1, e2] + es

# ArithExpr ==========================


NArithExpr |= NT1, NArithExprTail, func1
NArithExprTail |= NAddOp, NT1, NArithExprTail, func2
NArithExprTail |= lambda : {}
NT1 |= NT2, NT1Tail, func1
NT1Tail |= NMulOp, NT2, NT1Tail, func2
NT1Tail |= lambda : {}
NT2 |= '(', NBrBlock, ')'
NT2 |= NFuncC
NT2 |= NUMBER
NT2 |= IDENT
NBrBlock |= NArithExpr
NBrBlock |= NStatement
NAddOp |= KW_PLUS, lambda :KW_PLUS
NAddOp |= KW_MINUS, lambda :KW_MINUS
NMulOp |= KW_MUL, lambda :KW_MUL
NMulOp |= KW_DIV, lambda :KW_DIV

# Type =======================

NType |= NListType, ListType
NType |= NTupleType, TupleType
NType |= KW_INT, IntType
NType |= KW_BOOL, BoolType
NListType |= '[', NType, ']'
NTupleType |= '(', NType, ',', NType, NTupleTypeList, ')', lambda t1, t2, tlist: [t1, t2] + tlist
NTupleTypeList |= ',', NType, NTupleTypeList, lambda t, tlist: [t] + tlist
NTupleTypeList |= lambda: []

p = pe.Parser(NProgram)
assert p.is_lalr_one()
p.add_skipped_domain('--[^\n]*\n')
p.add_skipped_domain('\\s')

# ============= Output =======================

from pprint import pprint
# import sys

filename = 'input.txt'
try:
    with open(filename) as f:
        tree = p.parse(f.read())
        print('OK')
        with open("output.txt", "w") as f:
            pprint(tree, stream=f)
except pe.Error as e:
    print(f'Ошибка {e.pos}: {e.message}')
except Exception as e:
    print('FAIL')
    print(e)
