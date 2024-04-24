#include "parser.hpp"
#include "node.hpp"
#include "token.hpp"
#include "rule.hpp"

#include <sstream>

std::shared_ptr<Scanner> Parser::scanner = nullptr;

#define TERM_RULE(param) std::make_shared<TermRule>(param)
#define NTERM_RULE(param) std::make_shared<NTermRule>(param)
#define CONC_RULE(...) std::make_shared<ConcRule>(std::vector<std::shared_ptr<Rule>>{__VA_ARGS__})
#define STAR_RULE(param) std::make_shared<StarRule>(param)
#define PLUS_RULE(param) std::make_shared<PlusRule>(param)
#define OPT_RULE(param) std::make_shared<OptRule>(param)


std::shared_ptr<Node> Parser::parse() {
    auto root = NTERM_RULE(NON_TERM::Program)->parse()[0];
    auto last_token = Parser::getInstance().sc->cur_token;
    if (last_token.tag != TAG_END) {
        std::stringstream buffer;
        buffer << "Wrong tag\n\tReceived: " << last_token << std::endl;
        buffer << "\tExpected: " << Token::tag2string[TAG_END] << std::endl;
        throw std::runtime_error(buffer.str());
    }
    return root;
}

const std::function<NodesType(const NodesType&)> BinOpCallback = [](const NodesType& items) -> NodesType {
    std::vector<std::shared_ptr<Expr>> exprs;
    std::vector<std::shared_ptr<Term>> ops;
    for (auto& i : items) {
        auto expr = std::dynamic_pointer_cast<Expr>(i);
        if (expr) {
            exprs.push_back(expr);
            continue;
        }
        auto op = std::dynamic_pointer_cast<Term>(i);
        if (op) {
            ops.push_back(op);
            continue;
        }
    }
    auto expr = exprs[0];
    for (size_t i = 1; i < exprs.size(); ++i) {
        expr = std::make_shared<BinOp>(expr, ops[i - 1], exprs[i]);
    }
    return { expr };
    };

const std::function<NodesType(const NodesType&)> PassUpCallback = [](const NodesType& items) -> NodesType {
    return items;
};


const std::unordered_map<NON_TERM::TYPE, std::vector<Alternative>> Parser::rules{
    {NON_TERM::Program, {
        {
            PLUS_RULE(NTERM_RULE(NON_TERM::FuncD)),
            [](const NodesType& items) -> NodesType {
                std::vector<std::shared_ptr<FuncD>> result;
                for (auto& i : items) {
                    result.push_back(std::dynamic_pointer_cast<FuncD>(i));
                }
                return {std::make_shared<Program>(result)};
            }
        }
    }},

    {NON_TERM::FuncD, {
        {
            CONC_RULE(
                TERM_RULE(TAG_IDENT),
                TERM_RULE(TAG_LPAREN),
                OPT_RULE(NTERM_RULE(NON_TERM::FuncDParams)),
                TERM_RULE(TAG_RPAREN),
                TERM_RULE(TAG_COLON),
                NTERM_RULE(NON_TERM::Type),
                TERM_RULE(TAG_EQUAL),
                NTERM_RULE(NON_TERM::Expr),
                TERM_RULE(TAG_SEMICOLON),
            ),
            [](const NodesType& items) -> NodesType {
                auto ident = std::dynamic_pointer_cast<Term>(items[0]);
                auto params = std::dynamic_pointer_cast<FuncDParams>(items[2]);
                auto type = std::dynamic_pointer_cast<Type>(items[5]);
                auto expr = std::dynamic_pointer_cast<Expr>(items[7]);
                return {std::make_shared<FuncD>(ident->t.attr, params, type, expr)};
            }
        }
    }},

    {NON_TERM::FuncDParam, {
        {
            CONC_RULE(
                TERM_RULE(TAG_IDENT),
                TERM_RULE(TAG_COLON),
                NTERM_RULE(NON_TERM::Type)
            ),
            [](const NodesType& items) -> NodesType {
                auto term = std::dynamic_pointer_cast<Term>(items[0]);
                auto type = std::dynamic_pointer_cast<Type>(items[2]);
                return {std::make_shared<FuncDParam>(term->t.attr, type)};
            }
        }
    }},

    {NON_TERM::FuncDParams, {
        {
            CONC_RULE(
                NTERM_RULE(NON_TERM::FuncDParam),
                STAR_RULE(CONC_RULE(TERM_RULE(TAG_COMMA), NTERM_RULE(NON_TERM::FuncDParam)))
            ),
            [](const NodesType& items) -> NodesType {
                std::vector<std::shared_ptr<FuncDParam>> result;
                for (auto& i : items) {
                    auto param = std::dynamic_pointer_cast<FuncDParam>(i);
                    if (param) {
                        result.push_back(param);
                    }
                }
                return {std::make_shared<FuncDParams>(result)};
            }
        }
    }},

    {NON_TERM::Expr, {
        {
            NTERM_RULE(NON_TERM::IfStatement),
            PassUpCallback
        },
        {
            NTERM_RULE(NON_TERM::ArithExpr),
            PassUpCallback
        },
    }},

    {NON_TERM::Exprs, {
        {
            CONC_RULE(
                NTERM_RULE(NON_TERM::Expr),
                STAR_RULE(CONC_RULE(TERM_RULE(TAG_COMMA), NTERM_RULE(NON_TERM::Expr))),
            ),
            [](const NodesType& items) -> NodesType {
                std::vector<std::shared_ptr<Node>> result;
                for (auto& i : items) {
                    auto expr = std::dynamic_pointer_cast<Expr>(i);
                    if (expr) {
                        result.push_back(i);
                    }
                }
                return result;
            }
        }
    }},


    {NON_TERM::IfStatement, {
        {
            CONC_RULE(
                TERM_RULE(TAG_IF),
                NTERM_RULE(NON_TERM::Expr),
                TERM_RULE(TAG_THEN),
                NTERM_RULE(NON_TERM::Expr),
                TERM_RULE(TAG_ELSE),
                NTERM_RULE(NON_TERM::Expr),
            ),
            [](const NodesType& items) -> NodesType {
                auto condition = std::dynamic_pointer_cast<Expr>(items[1]);
                auto true_br = std::dynamic_pointer_cast<Expr>(items[3]);
                auto false_br = std::dynamic_pointer_cast<Expr>(items[5]);
                return { std::make_shared<IfStatement>(condition, true_br, false_br) };
            }
        }
    }},

    {NON_TERM::Var, {
        {
            CONC_RULE(
                TERM_RULE(TAG_IDENT),
                OPT_RULE(CONC_RULE(
                    TERM_RULE(TAG_LPAREN),
                    OPT_RULE(NTERM_RULE(NON_TERM::Exprs)),
                    TERM_RULE(TAG_RPAREN)
                ))
            ),
            [](const NodesType& items) -> NodesType {
                auto name = std::dynamic_pointer_cast<Term>(items[0]);
                if (items.size() == 1) {
                    return {std::make_shared<Var>(name)};
                }
                std::vector<std::shared_ptr<Expr>> exprs;
                for (auto& i : items) {
                    auto expr = std::dynamic_pointer_cast<Expr>(i);
                    if (expr) {
                        exprs.push_back(expr);
                    }
                }
                return {std::make_shared<FuncCall>(name, exprs)};
            }
        }
    }},

    {NON_TERM::Type, {
        {
            CONC_RULE(
            TERM_RULE(TAG_LQPAREN),
            NTERM_RULE(NON_TERM::Type),
            TERM_RULE(TAG_RQPAREN),
            ),

            [](const NodesType& items) -> NodesType {
                return {std::make_shared<ListType>(std::dynamic_pointer_cast<Type>(items[1]))};
            }
        },
        {
            CONC_RULE(
                TERM_RULE(TAG_LPAREN),
                NTERM_RULE(NON_TERM::Type),
                TERM_RULE(TAG_COMMA),
                NTERM_RULE(NON_TERM::Type),
                STAR_RULE(CONC_RULE(TERM_RULE(TAG_COMMA), NTERM_RULE(NON_TERM::Type))),
                TERM_RULE(TAG_RPAREN)
            ),
            [](const NodesType& items) -> NodesType {
                std::vector<std::shared_ptr<Type>> types;
                for (auto& i : items) {
                    auto type = std::dynamic_pointer_cast<Type>(i);
                    if (type) {
                        types.push_back(type);
                    }
                }
                return {std::make_shared<TupleType>(types)};
            }
        },
        {
            TERM_RULE(TAG_TYPE),
            [](const NodesType& items) -> NodesType {
                return {std::make_shared<SimpleType>(std::dynamic_pointer_cast<Term>(items[0])->t.attr)};
            }
        }
    }},


    {NON_TERM::NumberLit, {
        {
            TERM_RULE(TAG_NUMBER),
            [](const NodesType& items) -> NodesType {
                auto attr = std::dynamic_pointer_cast<Term>(items[0])->t.attr;
                return {std::make_shared<Num>(atoll(attr.c_str()))};
            }
        }
    }},

    {NON_TERM::BoolLit, {
        {
            TERM_RULE(TAG_BOOL_NUMBER),
            [](const NodesType& items) -> NodesType {
                auto attr = std::dynamic_pointer_cast<Term>(items[0])->t.attr;
                return {std::make_shared<BoolNum>((attr == "true" ? true : false))};
            }
        }
    }},

    {NON_TERM::Parens, {
        {
            CONC_RULE(
                TERM_RULE(TAG_LPAREN),
                NTERM_RULE(NON_TERM::Expr),
                OPT_RULE(
                    CONC_RULE(
                        TERM_RULE(TAG_COMMA),
                        NTERM_RULE(NON_TERM::Exprs),
                    )
                ),
                TERM_RULE(TAG_RPAREN),
            ),
            [](const NodesType& items) -> NodesType {
                std::vector<std::shared_ptr<Expr>> exprs;
                for (auto& i : items) {
                    auto expr = std::dynamic_pointer_cast<Expr>(i);
                    if (expr) {
                        exprs.push_back(expr);
                    }
                }
                if (exprs.size() == 1) {
                    return {exprs[0]};
                }
                return {std::make_shared<TupleLit>(exprs)};
            }
        }
    }},

    {NON_TERM::ListLit, {
        {
            CONC_RULE(
                TERM_RULE(TAG_LQPAREN),
                OPT_RULE(NTERM_RULE(NON_TERM::Exprs)),
                TERM_RULE(TAG_RQPAREN),
            ),
            [](const NodesType& items) -> NodesType {
                std::vector<std::shared_ptr<Expr>> exprs;
                for (auto& i : items) {
                    auto expr = std::dynamic_pointer_cast<Expr>(i);
                    if (expr) {
                        exprs.push_back(expr);
                    }
                }
                return {std::make_shared<ListLit>(exprs)};
            }
        }
    }},

    {NON_TERM::ArithExpr, {
        {
            CONC_RULE(
                NTERM_RULE(NON_TERM::T1),
                STAR_RULE(CONC_RULE(
                    NTERM_RULE(NON_TERM::T1Op),
                    NTERM_RULE(NON_TERM::T1))
                )
            ),
            BinOpCallback
        }
    }},

    {NON_TERM::T1Op, {
        {
            TERM_RULE(TAG_PLUS),
            PassUpCallback
        },
        {
            TERM_RULE(TAG_MINUS),
            PassUpCallback
        },
        {
            TERM_RULE(TAG_OR),
            PassUpCallback
        },
    }},

    {NON_TERM::T1, {
        {
            CONC_RULE(
                NTERM_RULE(NON_TERM::T2),
                STAR_RULE(CONC_RULE(
                    NTERM_RULE(NON_TERM::T2Op),
                    NTERM_RULE(NON_TERM::T2))
                )
            ),
            BinOpCallback
        }
    }},

    {NON_TERM::T2Op, {
        {
            TERM_RULE(TAG_MULT),
            PassUpCallback
        },
        {
            TERM_RULE(TAG_DIV),
            PassUpCallback
        },
        {
            TERM_RULE(TAG_AND),
            PassUpCallback
        },
    }},

    {NON_TERM::T2, {
        {
            CONC_RULE(
                OPT_RULE(
                    NTERM_RULE(NON_TERM::T3Op)
                ),
                NTERM_RULE(NON_TERM::T3)
            ),
            [](const NodesType& items) -> NodesType {
                if (items.size() == 2) {
                    auto term = std::dynamic_pointer_cast<Term>(items[0]);
                    auto expr = std::dynamic_pointer_cast<Expr>(items[1]);
                    return {std::make_shared<UnOp>(term, expr)};
                }
                return items;
            }
        }
    }},

    {NON_TERM::T3Op, {
        {
            TERM_RULE(TAG_MINUS),
            PassUpCallback
        },
        {
            TERM_RULE(TAG_NOT),
            PassUpCallback
        },
    }},

    {NON_TERM::T3, {
        {
            NTERM_RULE(NON_TERM::Var),
            PassUpCallback
        },
        {
            NTERM_RULE(NON_TERM::NumberLit),
            PassUpCallback
        },
        {
            NTERM_RULE(NON_TERM::ListLit),
            PassUpCallback
        },
        {
            NTERM_RULE(NON_TERM::BoolLit),
            PassUpCallback
        },
        {
            NTERM_RULE(NON_TERM::Parens),
            PassUpCallback
        }
    }},
};

