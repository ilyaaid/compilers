from dataclasses import dataclass

# Type =====================

@dataclass
class Type:
    pass

@dataclass
class IntType(Type):
    pass

@dataclass
class BoolType(Type):
    pass

@dataclass
class ListType(Type):
    type: Type

@dataclass
class TupleType(Type):
    types: list[Type]


# Expr ================
class Expr():
    pass

@dataclass
class ListLit(Expr):
    elems: list[Expr]

@dataclass
class TupleLit(Expr):
    elems: list[Expr]

@dataclass
class FuncC(Expr):
    name: str
    params: list[Expr]


# Statement ===============

class Statement(Expr):
    pass

@dataclass
class IfStatement(Statement):
    condition : Expr
    then_branch : Expr
    else_branch : Expr


@dataclass
class BinOpExpr(Expr):
    op1: Expr
    op: str
    op2: Expr

# VarDef ==========================
    
@dataclass
class VarDef:
    name : str
    type : Type

@dataclass
class VarDefList:
    vars : list[VarDef]

@dataclass
class FuncDef:
    name : str
    params : VarDefList
    retType: Type
    expr: Expr

# Program =======================

@dataclass
class Program:
    funcDs: list[FuncDef]
