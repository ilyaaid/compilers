#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <stack>

#include "location.hh"


class Program;
class Function;
struct FunctionType;
class VarDef;
class Type;
class Expr;


class Program {
    std::vector<std::shared_ptr<Function>> funcs;
    ps::location loc;

    void addDefaultFuncs();

public:
    Program(std::vector<std::shared_ptr<Function>> funcs, ps::location loc): 
    funcs(funcs), loc(loc) {}

    void check();

    static std::map<std::string, FunctionType> func_name2func_type;
    static std::stack<std::string> cur_func_name;

};

struct FunctionType {
    std::map<std::string, size_t> param_name2param_ind;
    std::vector<std::shared_ptr<Type>> params_type;
    std::shared_ptr<Type> ret_type;
};

class Function {
    std::string name;
    std::vector<std::shared_ptr<VarDef>> params;
    std::shared_ptr<Type> ret_type;
    std::shared_ptr<Expr> expr;
    ps::location loc;

public:
    Function(std::string name,
        std::vector<std::shared_ptr<VarDef>> params,
        std::shared_ptr<Type> ret_type,
        std::shared_ptr<Expr> expr,
        ps::location loc):
        name(name),
        params(params),
        ret_type(ret_type),
        expr(expr), 
        loc(loc)  {}

    std::string getName() const;
    FunctionType getType() const;

    void check();
};


class VarDef {
    std::string name;
    std::shared_ptr<Type> type;
    ps::location loc;

public:
    VarDef(std::string name,
        std::shared_ptr<Type> type,
        ps::location loc):
        name(name), type(type), loc(loc)  {}

    std::string getName() const;
    std::shared_ptr<Type> getType() const;
};


/*========================= Expr =====================*/

class Expr {
public:
    virtual ~Expr() = default;

    virtual std::shared_ptr<Type> getType() = 0;
};

class CallFunction : public Expr {
    std::string name;
    std::vector<std::shared_ptr<Expr>> params;

    ps::location loc;

public:
    CallFunction(std::string name,
        std::vector<std::shared_ptr<Expr>> params,
        ps::location loc):
        name(name), params(params), loc(loc) {}

    std::shared_ptr<Type> getType() override;
};

class IfExpr : public Expr {
    std::shared_ptr<Expr> cond;
    std::shared_ptr<Expr> then_br;
    std::shared_ptr<Expr> else_br;
    ps::location loc;

public:
    IfExpr(std::shared_ptr<Expr> cond,
        std::shared_ptr<Expr> then_br,
        std::shared_ptr<Expr> else_br,
        ps::location loc):
        cond(cond),
        then_br(then_br),
        else_br(else_br), 
        loc(loc) {}

    std::shared_ptr<Type> getType() override;
};


/* =================== Operations ================*/

class BinOpExpr : public Expr {
    std::shared_ptr<Expr> lhs;
    std::string op;
    std::shared_ptr<Expr> rhs;
    ps::location loc;

public:
    BinOpExpr(std::shared_ptr<Expr> lhs,
        std::string op,
        std::shared_ptr<Expr> rhs,
        ps::location loc):
        lhs(lhs),
        op(op),
        rhs(rhs),
        loc(loc) {}

    std::shared_ptr<Type> getType() override;
};

class UnaryOpExpr : public Expr {
    std::string op;
    std::shared_ptr<Expr> rhs;
    ps::location loc;

public:
    UnaryOpExpr(
        std::string op,
        std::shared_ptr<Expr> rhs,
        ps::location loc):
        op(op),
        rhs(rhs),
        loc(loc) {}

    std::shared_ptr<Type> getType() override;
};


/*============ Lits ===================*/

class Lit : public Expr {};

class NumberLit : public Lit {
    int num;
    ps::location loc;
public:
    NumberLit(int num,
    ps::location loc): num(num), loc(loc) {}

    std::shared_ptr<Type> getType() override;
};

class BoolNumberLit : public Lit {
    bool num;
    ps::location loc;
public:
    BoolNumberLit(bool num,
    ps::location loc): num(num), loc(loc) {}

    std::shared_ptr<Type> getType() override;
};

class ListLit : public Lit {
    std::vector<std::shared_ptr<Expr>> elems;
    ps::location loc;

public:
    ListLit(std::vector<std::shared_ptr<Expr>> elems,
    ps::location loc):
        elems(elems),loc(loc) {}
    
    std::shared_ptr<Type> getType() override;
};

class TupleLit : public Lit {
    std::vector<std::shared_ptr<Expr>> elems;
    ps::location loc;

public:
    TupleLit(std::vector<std::shared_ptr<Expr>> elems,
    ps::location loc):
        elems(elems), loc(loc) {}

    std::shared_ptr<Type> getType() override;
};

class VariableLit : public Lit {
    std::string name;
    ps::location loc;
public:
    VariableLit(std::string name,
    ps::location loc): name(name), loc(loc) {}

    std::shared_ptr<Type> getType() override;
};


/*=====================Types===============*/

class Type {
public:
    virtual ~Type() = default;

    virtual bool operator == (std::shared_ptr<Type> rhs) const = 0;
    virtual bool operator != (std::shared_ptr<Type> rhs) const = 0;
};

class AnyType : public Type {
public:
    bool operator == (std::shared_ptr<Type> rhs) const override;
    bool operator != (std::shared_ptr<Type> rhs) const override;
};

class SimpleType : public Type {
public:
    enum TYPE {
        BOOL,
        INT
    };

    SimpleType(TYPE type): type(type) {}
    
    bool operator == (std::shared_ptr<Type> rhs) const override;
    bool operator != (std::shared_ptr<Type> rhs) const override;

private:
    TYPE type;
};

class ListType : public Type {
    std::shared_ptr<Type> type;
public:
    ListType(std::shared_ptr<Type> type):
        type(type) {}

    bool operator == (std::shared_ptr<Type> rhs) const override;
    bool operator != (std::shared_ptr<Type> rhs) const override;
};

class TupleType : public Type {
    std::vector<std::shared_ptr<Type>> types;
public:
    TupleType(std::vector<std::shared_ptr<Type>> types):
        types(types) {}

    bool operator == (std::shared_ptr<Type> rhs) const override;
    bool operator != (std::shared_ptr<Type> rhs) const override;
};
