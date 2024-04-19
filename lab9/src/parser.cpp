#include "parser.hpp"
#include "node.hpp"
#include "token.hpp"
#include "rule.hpp"

#include <tuple>
#include <stack>
#include <sstream>

std::shared_ptr<Scanner> Parser::scanner = nullptr;

#define TERM_RULE(param) std::make_shared<TermRule>(param)
#define NTERM_RULE(param) std::make_shared<NTermRule>(param)
#define CONC_RULE(...) std::make_shared<ConcRule>(std::vector<std::shared_ptr<Rule>>{__VA_ARGS__})
#define ALT_RULE(...) std::make_shared<AltRule>(std::vector<std::shared_ptr<Rule>>{__VA_ARGS__})
#define STAR_RULE(param) std::make_shared<StarRule>(param)
#define PLUS_RULE(param) std::make_shared<PlusRule>(param)
#define OPT_RULE(param) std::make_shared<OptRule>(param)

const std::unordered_map<NON_TERM, std::shared_ptr<Rule>> Parser::rules{
    {Program, PLUS_RULE(NTERM_RULE(FuncD))},

    {FuncD, CONC_RULE(
        TERM_RULE(TAG_IDENT),
        TERM_RULE(TAG_LPAREN),
        OPT_RULE(NTERM_RULE(FuncDParams)),
        TERM_RULE(TAG_RPAREN),
        TERM_RULE(TAG_COLON),
        NTERM_RULE(Type),
        TERM_RULE(TAG_EQUAL),
        NTERM_RULE(Expr),
        TERM_RULE(TAG_SEMICOLON),
    )},

    {FuncDParams, CONC_RULE(
        NTERM_RULE(FuncDParam),
        STAR_RULE(CONC_RULE(TERM_RULE(TAG_COMMA), NTERM_RULE(FuncDParam)))
    )},

    {FuncDParam, CONC_RULE(
        TERM_RULE(TAG_IDENT),
        TERM_RULE(TAG_COLON),
        NTERM_RULE(Type)
    )},

    {Statement, NTERM_RULE(IfStatement)},

    {IfStatement, CONC_RULE(
        TERM_RULE(TAG_IF),
        NTERM_RULE(LogicExpr),
        TERM_RULE(TAG_THEN),
        NTERM_RULE(Expr),
        TERM_RULE(TAG_ELSE),
        NTERM_RULE(Expr),
    )},

    {Expr, ALT_RULE(
        NTERM_RULE(Statement),
        NTERM_RULE(ArithExpr),
        NTERM_RULE(TupleLit),
        NTERM_RULE(ListLit),
    )},

    {Exprs, CONC_RULE(
        NTERM_RULE(Expr),
        STAR_RULE(CONC_RULE(TERM_RULE(TAG_COMMA), NTERM_RULE(Expr))),
    )},

    {Var, CONC_RULE(
        TERM_RULE(TAG_IDENT),
        OPT_RULE(CONC_RULE(
            TERM_RULE(TAG_LPAREN),
            OPT_RULE(NTERM_RULE(Exprs)),
            TERM_RULE(TAG_RPAREN)
        ))
    )},

    {Type, ALT_RULE(
        CONC_RULE(
            TERM_RULE(TAG_LQPAREN),
            NTERM_RULE(Type),
            TERM_RULE(TAG_RQPAREN),
        ),
        CONC_RULE(
            TERM_RULE(TAG_LPAREN),
            NTERM_RULE(Type),
            TERM_RULE(TAG_COMMA),
            NTERM_RULE(Types),
            TERM_RULE(TAG_RPAREN)
        ),
        TERM_RULE(TAG_TYPE),
    )},

    {Types, CONC_RULE(
        NTERM_RULE(Type),
        STAR_RULE(CONC_RULE(TERM_RULE(TAG_COLON), NTERM_RULE(Type))),
    )},

    {NumberLit, TERM_RULE(TAG_NUMBER)},

    {BoolLit, TERM_RULE(TAG_BOOL_NUMBER)},

    {TupleLit, CONC_RULE(
        TERM_RULE(TAG_LPAREN),
        NTERM_RULE(Expr),
        TERM_RULE(TAG_COMMA),
        NTERM_RULE(Exprs),
        TERM_RULE(TAG_RPAREN),
    )},

    {ListLit, CONC_RULE(
        TERM_RULE(TAG_LQPAREN),
        OPT_RULE(NTERM_RULE(Exprs)),
        TERM_RULE(TAG_RQPAREN),
    )},

    {ArithExpr, CONC_RULE(
        NTERM_RULE(ArithT1),
        STAR_RULE(CONC_RULE(
            ALT_RULE(TERM_RULE(TAG_PLUS),TERM_RULE(TAG_MINUS)),
            NTERM_RULE(ArithT1))
        )
    )},

    {ArithT1, CONC_RULE(
        NTERM_RULE(ArithT2),
        STAR_RULE(CONC_RULE(
            ALT_RULE(TERM_RULE(TAG_MULT),TERM_RULE(TAG_DIV)),
            NTERM_RULE(ArithT2))
        )
    )},

    {ArithT2, CONC_RULE(
        OPT_RULE(TERM_RULE(TAG_MINUS)),
        NTERM_RULE(ArithT3)
    )},

    {ArithT3, ALT_RULE(
        NTERM_RULE(Var),
        NTERM_RULE(NumberLit),
    )},

    {LogicExpr, CONC_RULE(
        NTERM_RULE(LogicT1),
        STAR_RULE(CONC_RULE(
            TERM_RULE(TAG_OR),
            NTERM_RULE(LogicT1)
        ))
    )},

    {LogicT1, CONC_RULE(
        NTERM_RULE(LogicT2),
        STAR_RULE(CONC_RULE(
            TERM_RULE(TAG_AND),
            NTERM_RULE(LogicT2)
        ))
    )},

    {LogicT2, CONC_RULE(
        OPT_RULE(TERM_RULE(TAG_NOT)),
        NTERM_RULE(LogicT3),
    )},

    {LogicT3, ALT_RULE(
        NTERM_RULE(Var),
        NTERM_RULE(BoolLit),
    )}
};

std::shared_ptr<Node> Parser::parse() {
    auto root = NTERM_RULE(Program)->parse()[0];
    auto last_token = Parser::getInstance().sc->cur_token;
    if (last_token.tag != TAG_END) {
        std::stringstream buffer;
        buffer << last_token << std::endl;
        throw std::runtime_error("wrong tag: " + buffer.str());
    }
    return root;
}

