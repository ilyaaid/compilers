#include "ast_tree.hpp"

#include <set>
#include <stdexcept>
#include <iostream>
#include <sstream>

std::string loc2str(const ps::location& loc) {
    std::stringstream sstr;
    sstr << loc;
    return sstr.str();
}

std::map<std::string, FunctionType> Program::func_name2func_type = {};

std::stack<std::string> Program::cur_func_name = {};

void Program::addDefaultFuncs()
{
    func_name2func_type.insert({ "null", {
        {{"xs", 0}},
        {std::make_shared<ListType>(std::make_shared<AnyType>())},
        std::make_shared<SimpleType>(SimpleType::BOOL)
    } });

    func_name2func_type.insert({ "car", {
        {{"xs", 0}},
        {std::make_shared<ListType>(std::make_shared<AnyType>())},
        std::make_shared<AnyType>()
    } });

    func_name2func_type.insert({ "cdr", {
        {{"xs", 0}},
        {std::make_shared<ListType>(std::make_shared<AnyType>())},
        std::make_shared<ListType>(std::make_shared<AnyType>())
    } });

    func_name2func_type.insert({ "cons", {
            {{"x", 0}, {"xs", 0}},
            {std::make_shared<AnyType>(), std::make_shared<ListType>(std::make_shared<AnyType>())},
            std::make_shared<ListType>(std::make_shared<AnyType>())
        } });
}

void Program::check()
{
    func_name2func_type.clear();

    addDefaultFuncs();

    std::set<std::string> names;
    for (auto& func : funcs) {
        std::string func_name = func->getName();
        if (names.find(func_name) != names.end()) {
            throw std::runtime_error("function \"" + func_name + "\" is already exists" + ":" + loc2str(loc));
        }
        names.insert(func_name);

        func_name2func_type[func->getName()] = func->getType();
    }

    for (auto& func : funcs) {
        cur_func_name.push(func->getName());
        func->check();
        cur_func_name.pop();
    }
}


std::string Function::getName() const
{
    return name;
}

FunctionType Function::getType() const
{
    std::map<std::string, size_t> param_name2param_ind;
    std::vector<std::shared_ptr<Type>> params_type;
    for (auto& param : params) {
        params_type.push_back(param->getType());
        param_name2param_ind[param->getName()] = params_type.size() - 1;
    }
    return { param_name2param_ind, params_type, ret_type };
}

void Function::check()
{

    std::set<std::string> names;
    for (auto& param : params) {
        std::string name = param->getName();
        if (names.find(name) != names.end()) {
            throw std::runtime_error("function \"" + this->name + "\": param \"" + name + "\" is already exists" + ":" + loc2str(loc));
        }
        names.insert(name);
    }
    expr->getType();
}

std::string VarDef::getName() const
{
    return name;
}

std::shared_ptr<Type> VarDef::getType() const
{
    return type;
}


/*==================== Exprs =======================*/

std::shared_ptr<Type> CallFunction::getType()
{
    if (Program::func_name2func_type.find(name) == Program::func_name2func_type.end()) {
        throw std::runtime_error("the called function \"" + name + "\" does not exist" + ":" + loc2str(loc));
    }

    auto func_type = Program::func_name2func_type[name];
    if (func_type.params_type.size() != params.size()) {
        throw std::runtime_error("Wrong number of arguments in called function \"" + name + "\"" + ":" + loc2str(loc));
    }
    for (size_t i = 0; i < params.size(); ++i) {
        if (*params[i]->getType() != func_type.params_type[i]) {
            throw std::runtime_error("Wrong params in called function \"" + name + "\"" + ":" + loc2str(loc));
        }
    }
    return Program::func_name2func_type[name].ret_type;
}


std::shared_ptr<Type> IfExpr::getType()
{
    auto cond_type = cond->getType();
    if (*cond_type != std::make_shared<SimpleType>(SimpleType::BOOL)) {
        throw std::runtime_error("if condition has not bool type:" + loc2str(loc));
    }
    auto then_type = then_br->getType();
    auto else_type = else_br->getType();
    if (*then_type != else_type) {
        throw std::runtime_error("then and else have different types:" + loc2str(loc));
    }
    return then_type;
}


std::shared_ptr<Type> BinOpExpr::getType()
{
    auto lhs_t = lhs->getType();
    auto rhs_t = rhs->getType();
    if (op == "or" || op == "and") {
        // логическое выражение
        auto bool_t = std::make_shared<SimpleType>(SimpleType::BOOL);
        if (*lhs_t != bool_t || *rhs_t != bool_t) {
            throw std::runtime_error("not bool operands in bin logical expr:" + loc2str(loc));
        }
        return bool_t;
    }
    // арифметическое выражение
    auto int_t = std::make_shared<SimpleType>(SimpleType::INT);
    if (*lhs_t != int_t || *rhs_t != int_t) {
        throw std::runtime_error("not int operands in bin arithmetic expr:" + loc2str(loc));
    }
    return int_t;
}


std::shared_ptr<Type> UnaryOpExpr::getType()
{
    auto rhs_t = rhs->getType();
    if (op == "not") {
        // логическое выражение
        auto bool_t = std::make_shared<SimpleType>(SimpleType::BOOL);
        if (*rhs_t != bool_t) {
            throw std::runtime_error("not bool operand in unary logical expr:" + loc2str(loc));
        }
        return bool_t;
    }
    // арифметическое выражение
    auto int_t = std::make_shared<SimpleType>(SimpleType::INT);
    if (*rhs_t != int_t) {
        throw std::runtime_error("not int operand in unary arithmetic expr:" + loc2str(loc));
    }
    return int_t;
}


/*===================== Lits =========================*/

std::shared_ptr<Type> NumberLit::getType()
{
    return std::make_shared<SimpleType>(SimpleType::TYPE::INT);
}


std::shared_ptr<Type> BoolNumberLit::getType()
{
    return std::make_shared<SimpleType>(SimpleType::TYPE::BOOL);
}


std::shared_ptr<Type> ListLit::getType()
{
    if (elems.empty()) {
        return std::make_shared<ListType>(std::make_shared<AnyType>());
    }
    auto type = elems[0]->getType();
    for (size_t i = 1; i < elems.size(); ++i) {
        if (*type != elems[i]->getType()) {
            throw std::runtime_error("wrong type of elements in list:" + loc2str(loc));
        }
    }
    return std::make_shared<ListType>(type);
}

std::shared_ptr<Type> TupleLit::getType()
{
    std::vector<std::shared_ptr<Type>> types;
    for (auto& elem : elems) {
        types.push_back(elem->getType());
    }
    return std::make_shared<TupleType>(types);
}

std::shared_ptr<Type> VariableLit::getType()
{
    auto cur_func_name = Program::cur_func_name.top();
    auto func_type = Program::func_name2func_type[cur_func_name];
    if (func_type.param_name2param_ind.find(name) == func_type.param_name2param_ind.end()) {
        throw std::runtime_error("unknown variable \"" + name + "\" in function \"" + cur_func_name + "\"" + ":" + loc2str(loc));
    }

    return std::shared_ptr<Type>(func_type.params_type[func_type.param_name2param_ind[name]]);
}



/*======================= Types =======================*/

bool AnyType::operator==(std::shared_ptr<Type> rhs) const
{
    return true;
}

bool AnyType::operator!=(std::shared_ptr<Type> rhs) const
{
    return !(*this == rhs);
}

bool SimpleType::operator==(std::shared_ptr<Type> rhs) const
{
    if (std::dynamic_pointer_cast<AnyType>(rhs)) {
        return true;
    }

    auto rhs_t = std::dynamic_pointer_cast<SimpleType>(rhs);
    return rhs_t && type == rhs_t->type;
}

bool SimpleType::operator!=(std::shared_ptr<Type> rhs) const
{
    return !(*this == rhs);
}

bool ListType::operator==(std::shared_ptr<Type> rhs) const
{
    if (std::dynamic_pointer_cast<AnyType>(rhs)) {
        return true;
    }

    auto rhs_t = std::dynamic_pointer_cast<ListType>(rhs);
    return rhs_t && *type == rhs_t->type;
}

bool ListType::operator!=(std::shared_ptr<Type> rhs) const
{
    return !(*this == rhs);
}

bool TupleType::operator==(std::shared_ptr<Type> rhs) const
{
    if (std::dynamic_pointer_cast<AnyType>(rhs)) {
        return true;
    }

    auto rhs_t = std::dynamic_pointer_cast<TupleType>(rhs);
    if (!rhs_t) {
        return false;
    }

    for (size_t i = 0; i < types.size(); ++i) {
        if (*types[i] != rhs_t->types[i]) {
            return false;
        }
    }
    return true;
}

bool TupleType::operator!=(std::shared_ptr<Type> rhs) const
{
    return !(*this == rhs);
}
