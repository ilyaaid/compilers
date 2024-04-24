#pragma once

#include "token.hpp"

#include <vector>
#include <memory>
#include <unordered_map>

struct NON_TERM {
    enum TYPE
    {
        Program,
        FuncD,
        FuncDParams,
        FuncDParam,

        Statement,
        IfStatement,

        Expr,
        Exprs,
        Var,

        Type,

        NumberLit,
        BoolLit,
        ListLit,
        Parens,

        ArithExpr,
        T1,
        T1Op,
        T2,
        T2Op,
        T3,
        T3Op
    };
};

class Node;
class Term;
class Program;
class FuncD;
class FuncDParam;
class FuncDParams;
class Expr;
class TupleLit;
class Type;

using NodesType = std::vector<std::shared_ptr<Node>>;

class Node {
public:
    virtual ~Node() {}

    virtual void print(std::ostream& out, int n) = 0;
};

class Term : public Node {
public:
    Term(const Token& t): t(t) {}
    void print(std::ostream& out, int n) override;
public:
    Token t;
};


class Program final : public Node {
public:
    Program(const std::vector<std::shared_ptr<FuncD>>& items): items(items) {}
    void print(std::ostream& out, int n) override;
private:
    std::vector<std::shared_ptr<FuncD>> items;
};

//==============FuncD=======================

class FuncD final : public Node {
public:
    FuncD(const std::string& name,
        const std::shared_ptr<FuncDParams>& params,
        const std::shared_ptr<Type>& return_type,
        const std::shared_ptr<Expr>& expr): name(name), params(params), return_type(return_type), expr(expr) {}
    void print(std::ostream& out, int n) override;
private:
    std::string name;
    std::shared_ptr<FuncDParams> params;
    std::shared_ptr<Type> return_type;
    std::shared_ptr<Expr> expr;
};

class FuncDParam final : public Node {
public:
    FuncDParam(const std::string& name,
        const std::shared_ptr<Type>& type): name(name), type(type) {}
    void print(std::ostream& out, int n) override;
private:
    std::string name;
    std::shared_ptr<Type> type;
};


class FuncDParams final : public Node {
public:
    FuncDParams(const std::vector<std::shared_ptr<FuncDParam>>& items): items(items) {}
    void print(std::ostream& out, int n) override;
private:
    std::vector<std::shared_ptr<FuncDParam>> items;
};

//===============Expr=====================

class Expr : public Node {};

class IfStatement final : public Expr {
public:
    IfStatement(const std::shared_ptr<Expr>& cond,
        const std::shared_ptr<Expr>& true_br,
        const std::shared_ptr<Expr>& false_br): cond(cond), true_br(true_br), false_br(false_br) {}
    void print(std::ostream& out, int n) override;
private:
    std::shared_ptr<Expr> cond;
    std::shared_ptr<Expr> true_br;
    std::shared_ptr<Expr> false_br;
};

class Var final : public Expr {
public:
    Var(const std::shared_ptr<Term>& val): val(val) {}
    void print(std::ostream& out, int n) override;
private:
    std::shared_ptr<Term> val;
};

class FuncCall final : public Expr {
public:
    FuncCall(const std::shared_ptr<Term>& val,
        const std::vector<std::shared_ptr<Expr>>& params): val(val), params(params) {}
    void print(std::ostream& out, int n) override;
private:
    std::shared_ptr<Term> val;
    std::vector<std::shared_ptr<Expr>> params;
};


//===============Type=====================

class Type : public Node {
};

class SimpleType final : public Type {
public:
    enum TYPE {
        INT,
        BOOL
    };
    SimpleType(const std::string& str): val(str) {}
    void print(std::ostream& out, int n) override;
public:
    std::string val;
};

class ListType final : public Type {
public:
    ListType(const std::shared_ptr<Type>& type): type(type) {}
    void print(std::ostream& out, int n) override;
public:
    std::shared_ptr<Type> type;
};

class TupleType final : public Type {
public:
    TupleType(const std::vector<std::shared_ptr<Type>>& items): items(items) {}
    void print(std::ostream& out, int n) override;
private:
    std::vector<std::shared_ptr<Type>> items;
};


//================Lits=====================

class TupleLit final : public Expr {
public:
    TupleLit(const std::vector<std::shared_ptr<Expr>>& exprs): exprs(exprs) {}
    void print(std::ostream& out, int n) override;
private:
    std::vector<std::shared_ptr<Expr>> exprs;
};

class ListLit final : public Expr {
public:
    ListLit(const std::vector<std::shared_ptr<Expr>>& exprs): exprs(exprs) {}
    void print(std::ostream& out, int n) override;
private:
    std::vector<std::shared_ptr<Expr>> exprs;
};

class Num final : public Expr {
public:
    Num(long long num): num(num) {}
    void print(std::ostream& out, int n) override;
private:
    long long num;
};


class BoolNum final : public Expr {
public:
    BoolNum(bool num): num(num) {}
    void print(std::ostream& out, int n) override;
private:
    bool num;
};


//===============Ops===============

class BinOp final : public Expr {
public:
    BinOp(const std::shared_ptr<Expr>& expr1,
        const std::shared_ptr<Term>& op,
        const std::shared_ptr<Expr>& expr2): expr1(expr1), op(op), expr2(expr2) {}
    void print(std::ostream& out, int n) override;
private:
    std::shared_ptr<Expr> expr1;
    std::shared_ptr<Term> op;
    std::shared_ptr<Expr> expr2;
};

class UnOp final : public Expr {
public:
    UnOp(const std::shared_ptr<Term>& op,
        const std::shared_ptr<Expr>& expr): op(op), expr(expr) {}
    void print(std::ostream& out, int n) override;
private:
    std::shared_ptr<Term> op;
    std::shared_ptr<Expr> expr;
};


