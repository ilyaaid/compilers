% Лабораторная работа № 3.1 «Самоприменимый генератор компиляторов
  на основе предсказывающего анализа»
% 22 мая 2024 г.
% Илья Алёшин, ИУ9-61Б

# Цель работы
Целью данной работы является изучение алгоритма построения таблиц предсказывающего анализатора.

# Индивидуальный вариант
```
/* аксиома помечена звёздочкой */
  F  ("n") ("(" E ")")
  T  (F T')
  T' ("*" F T') ()
* E  (T E')
  E' ("+" T E') ()
```

# Реализация

## Predictive analyzer
Файл parser.hpp
```cpp
#pragma once

#include "node.hpp"

#include <vector>
#include <memory>
#include <unordered_map>

struct ParseNode
{
  int tagnterm;
  std::shared_ptr<NTermNode> node;
};

class Parser
{
public:

  Parser(const std::vector<Token>& tokens);

  std::shared_ptr<Node> parse();
  
private:
  std::vector<Token> tokens;
};

```

Файл node.hpp
```cpp
#pragma once


#include "token.hpp"
#include "table.hpp"

#include <vector>
#include <memory>
#include <unordered_map>


struct Node
{
    virtual void print(int s) = 0;

    void printDOT(std::ostream &out);

    virtual void printDOT_(std::ostream &out_filename, int &n) = 0;
};

struct NTermNode : public Node
{
    NTermNode(NON_TERM nterm);

    void print(int s) override;
    // void printDOT(std::ostream &out) override;

    void printDOT_(std::ostream &out_filename, int &n) override;

public:
    NON_TERM nterm;
    std::vector<std::shared_ptr<Node>> children;

};

struct LeafNode : public Node
{
    LeafNode(const Token &token);

    void print(int s) override;
    // void printDOT(std::ostream &out) override;

    void printDOT_(std::ostream &out_filename, int &n) override;

public:
    Token token;
};

```

Файл position.hpp
```cpp
#pragma once

#include <iostream>

struct Position
{
    int line = 1;
    int pos = 1;
    int index = 1;

    void operator++(int);
    void new_line();
};

std::ostream &operator<<(std::ostream &out, const Position &pos);

struct Fragment
{
    Position start;
    Position end;
};

std::ostream &operator<<(std::ostream &out, const Fragment &frag);

```

Файл scanner.hpp
```cpp
#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include <string>
#include <variant>
#include <memory>
#include <functional>

#include "parser.hpp"

using return_type = DOMAIN_TAG;
using semantic_type = std::string;
using location_type = Fragment;

class Scanner : public yyFlexLexer
{
public:
    Scanner(std::istream *in, std::ostream *out) : yyFlexLexer(in, out){};
    virtual ~Scanner(){};

    using FlexLexer::yylex;

    return_type lex(std::shared_ptr<semantic_type> &lval, std::shared_ptr<location_type> &lloc);
};

class Driver
{
public:
    void common(const std::shared_ptr<semantic_type>& lval,
    const std::shared_ptr<location_type>& lloc,
    const semantic_type& yytext);

    return_type end(const std::shared_ptr<semantic_type>& lval);

public:
    std::string cur_text;

    Position cur_pos;
    bool continued = false;
};

```


Файл table.hpp
```cpp
#pragma once 
#include <vector>
#include <unordered_map>
#include "token.hpp" 

enum NON_TERM : int 
{ 
	// все терминалы имеют тип char, а значит значение < 256
	NON_TERM_ArgOR =256,
	NON_TERM_Axiom =257,
	NON_TERM_NTerm =258,
	NON_TERM_NTermRule =259,
	NON_TERM_NTermRuleTail =260,
	NON_TERM_Rule =261,
	NON_TERM_Rules =262,
	NON_TERM_SymArgOR =263,
	ROOT,
};

const NON_TERM Axiom = NON_TERM_Rules;

const std::unordered_map<int, const std::unordered_map<DOMAIN_TAG, std::vector<int>>> table {
	{ NON_TERM_ArgOR,{
		{')', {}},
		{'s', {NON_TERM_SymArgOR, NON_TERM_ArgOR}},
		{'i', {NON_TERM_SymArgOR, NON_TERM_ArgOR}},
	}},
	{ NON_TERM_NTermRuleTail,{
		{'i', {}},
		{'*', {}},
		{0, {}},
		{'(', {'(', NON_TERM_ArgOR, ')', NON_TERM_NTermRuleTail}},
	}},
	{ NON_TERM_NTermRule,{
		{'(', {'(', NON_TERM_ArgOR, ')', NON_TERM_NTermRuleTail}},
	}},
	{ NON_TERM_NTerm,{
		{'i', {'i'}},
	}},
	{ NON_TERM_SymArgOR,{
		{'i', {NON_TERM_NTerm}},
		{'s', {'s'}},
	}},
	{ NON_TERM_Rule,{
		{'i', {NON_TERM_Axiom, NON_TERM_NTerm, NON_TERM_NTermRule}},
		{'*', {NON_TERM_Axiom, NON_TERM_NTerm, NON_TERM_NTermRule}},
	}},
	{ NON_TERM_Axiom,{
		{'i', {}},
		{'*', {'*'}},
	}},
	{ NON_TERM_Rules,{
		{0, {}},
		{'i', {NON_TERM_Rule, NON_TERM_Rules}},
		{'*', {NON_TERM_Rule, NON_TERM_Rules}},
	}},
};

```


Файл token.hpp
```cpp
#pragma once

#include "position.hpp"

#include <unordered_map>

using DOMAIN_TAG = char;

struct Token
{
    Token() {}
    Token(DOMAIN_TAG tag, const std::string& attr, const Fragment& frag): tag(tag), attr(attr), frag(frag) {}

    DOMAIN_TAG tag;
    std::string attr;
    Fragment frag;

    bool operator ==(const Token& t) const
    {
        return tag == t.tag && attr == t.attr;
    }

    bool operator<(const Token& t) const {
        if (tag == t.tag) {
            return attr < t.attr;
        }
        return tag < t.tag;
    }
};


class TokenHash {
public:
    size_t operator()(const Token& t) const
    {
        return (std::hash<char>()(t.tag)) ^
            (std::hash<std::string>()(t.attr));
    }
};

std::ostream& operator<<(std::ostream& out, const Token& token);

```


Файл node.cpp
```cpp
#include "analyzer/node.hpp"

NTermNode::NTermNode(NON_TERM nterm): nterm(nterm) {}

LeafNode::LeafNode(const Token& token): token(token) {}

//===============================  DOT

void Node::printDOT(std::ostream& out) {
    out << "digraph G {" << std::endl;
    int n = 0;
    printDOT_(out, n);
    out << "}" << std::endl;
}

void NTermNode::printDOT_(std::ostream& out, int& n)
{
    out << n << " [label=\"" << nterm << "\"]" << std::endl;
    int parent = n;
    std::vector<int> children_nums;
    for (size_t i = 0; i < children.size(); ++i)
    {
        n++;
        out << parent << "->" << n << std::endl;
        children_nums.push_back(n);
        children[i]->printDOT_(out, n);
    }

    if (children_nums.size() > 1)
    {
        out << "{ rank=same; ";
        out << children_nums[0];
        for (size_t i = 1; i < children_nums.size(); ++i)
        {
            out << " -> " << children_nums[i];
        }
        out << " [style=invis] }" << std::endl;
    }
}

void LeafNode::printDOT_(std::ostream& out, int& n)
{
    out << n << " [label=\"" << token.tag << "\"]" << std::endl;
}

//===============================  print

void NTermNode::print(int s)
{
    for (int i = 0; i < s; ++i)
    {
        std::cout << "------";
    }
    std::cout << nterm << std::endl;
    for (size_t i = 0; i < children.size(); ++i)
    {
        children[i]->print(s + 1);
    }
}

void LeafNode::print(int s)
{
    for (int i = 0; i < s; ++i)
    {
        std::cout << "------";
    }
    std::cout << token << " " << std::endl;
}

```


Файл parser.cpp
```cpp
#include "analyzer/parser.hpp"

#include <tuple>
#include <stack>
#include <sstream>

Parser::Parser(const std::vector<Token>& tokens)
{
    this->tokens = tokens;
}

std::shared_ptr<Node> Parser::parse()
{
    std::stack<ParseNode> stack;

    auto root = std::make_shared<NTermNode>(NON_TERM::ROOT);
    stack.push({ Axiom, root });

    int token_ind = 0;
    while (!stack.empty()) {
        ParseNode cur = stack.top();
        stack.pop();

        if (cur.tagnterm < 256) {
            if (cur.tagnterm == tokens[token_ind].tag) {
                if (cur.tagnterm != 0) { // != TAG_END
                    auto new_child = std::make_shared<LeafNode>(tokens[token_ind]);
                    cur.node->children.push_back(new_child);
                }
                token_ind++;
            }
            else {
                std::stringstream sstr;
                sstr << "wrong tag: \n\treceived:" << tokens[token_ind].tag <<
                    "\n\texpected:" << cur.tagnterm;
                throw std::runtime_error(sstr.str());
            }
        }
        else {
            try {
                std::vector<int> next_rule = table.at(cur.tagnterm).at(tokens[token_ind].tag);
                auto new_parent = std::make_shared<NTermNode>(static_cast<NON_TERM>(cur.tagnterm));
                cur.node->children.push_back(new_parent);
                for (int i = next_rule.size() - 1; i >= 0; i--) {
                    stack.push({ next_rule[i], new_parent });
                }
            }
            catch (...) {
                std::stringstream sstr;
                sstr << "there is no transition through the table \n\t from: "
                    << cur.tagnterm << "\n\t by: " << tokens[token_ind].tag;
                throw std::runtime_error(sstr.str());
            }

        }
    }
    return root->children[0];
}

```


Файл position.cpp
```cpp
#include "analyzer/position.hpp"

std::ostream &operator<<(std::ostream &out, const Position &pos)
{
    out << "(" << pos.line << "," << pos.pos << ")";
    return out;
}

void Position::operator++(int) {
    index++;
    pos++;
}

void Position::new_line() {
    pos = 1;
    line++;
}


std::ostream &operator<<(std::ostream &out, const Fragment &frag)
{
    out << frag.start << "-" << frag.end;
    return (out);
}

```


Файл scanner.cpp
```cpp
#include "analyzer/scanner.hpp"

#include <iostream>

void Driver::common(const std::shared_ptr<semantic_type>& lval,
    const std::shared_ptr<location_type>& lloc,
    const semantic_type& yytext)
{
    if (!continued)
    {
        lloc->start = cur_pos;
        cur_text.clear();
    }
    continued = false;

    cur_text += yytext;
    for (size_t i = 0; i < yytext.size(); ++i)
    {
        cur_pos++;
        if (yytext[i] == '\n')
        {
            cur_pos.new_line();
        }
    }
    lloc->end = cur_pos;
    lval->assign(cur_text);
}

return_type Driver::end(const std::shared_ptr<semantic_type>& lval)
{
    return 0;
}


```

Файл token.cpp
```cpp
#include "analyzer/token.hpp"


std::ostream &operator<<(std::ostream &out, const Token &token)
{
    std::string str;
    try {
        str = token.tag;
    } catch(...) {
        str = "UNDEFINED";
    }
    out << str << "=" + token.attr << ":" << token.frag;
    return out;
}

```

## Generator
Файл lexer.flex
```cpp
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


IDENT [A-Z][A-Za-z]*\'*

%x COMMENT STRING

%%

{IDENT} {
   std::replace(lval->begin(), lval->end(), '\'', '_');
   return 'i';
   // return DOMAIN_TAG::TAG_IDENT;
}

\/\* {
   BEGIN(COMMENT);
   driver.continued = true;
}

<COMMENT>. {
   driver.continued = true;
}

<COMMENT>\*\/ {
   BEGIN(INITIAL);
}

<COMMENT><<EOF>> {
   return driver.end(lval);
}

\" {
   BEGIN(STRING);
   driver.continued = true;
}

<STRING>\" {
   BEGIN(0);
   lval->erase(lval->begin());
   lval->erase(lval->begin() + lval->length() - 1);
   return 's';
   // return DOMAIN_TAG::TAG_STRING;
}

<STRING>\n |

<STRING><<EOF>> {
   throw std::runtime_error("error in token STRING:\n\treceived:EOF\n\texpected:\"");
}

<STRING>. {
   driver.continued = true;
}

\( {
   return '(';
   // return DOMAIN_TAG::TAG_LPAREN;
}

\) {
   return ')';
   // return DOMAIN_TAG::TAG_RPAREN;
}

\* {
   return '*';
   // return DOMAIN_TAG::TAG_STAR;
}

[ \t\r\n]+ {
}

. {
   throw std::runtime_error("error token:\n\treceived:"+*lval);
}


%%

```

Файл table_generator.hpp
```cpp
#pragma once

#include <vector>
#include <set>
#include <string>

#include "analyzer/node.hpp"

struct Rule {
    bool axiom;
    Token nterm;
    std::vector<std::vector<Token>> next;
};

class TableGenerator {
public:
    TableGenerator(const std::shared_ptr<Node>& root): root(root) {}

    void generate(std::ostream& out);

private:
    void _genetateFile(std::ostream& out);

    void _findFirst(const std::vector<Rule>& rules);
    std::set<Token> _findFirstForRule(const std::vector<Token>& tokens);
    void _findFollow(const std::vector<Rule>& rules);
    void _findPredictTable(const std::vector<Rule>& rules);

    void _checkRules(const std::vector<Rule>& rules);

    void _readRules(const std::shared_ptr<Node>& node, std::vector<Rule>& rules);
    Rule _readRule(const std::shared_ptr<Node>& node);
    bool _readAxiom(const std::shared_ptr<Node>& node);
    Token _readNTerm(const std::shared_ptr<Node>& node);
    std::vector<std::vector<Token>> _readNTermRule(const std::shared_ptr<Node>& node);
    std::vector<Token> _readArgOR(const std::shared_ptr<Node>& node, std::vector<Token>& tokens);
    void _readNTermRuleTail(const std::shared_ptr<Node>& node, std::vector<std::vector<Token>>& argsOR);
    Token _readSymArgOR(const std::shared_ptr<Node>& node);

    std::set<Token> leftNTerms;
    std::set<Token> rightNTerms;

    const Token EPSILON = Token('e', "EPSILON", {});
    const Token END = Token(0, "", {});
    std::unordered_map<Token, std::set<Token>, TokenHash> first;
    std::unordered_map<Token, std::set<Token>, TokenHash> follow;

    std::unordered_map<Token, 
    std::unordered_map<Token, std::vector<Token>, 
    TokenHash>, 
    TokenHash> predict_table;

    Token axiom;

    std::shared_ptr<Node> root = nullptr;
};

```

Файл table_generator.cpp
```cpp
#include "table_generator.hpp"
#include <algorithm>

void TableGenerator::generate(std::ostream& out)
{
    std::vector<Rule> rules;
    _readRules(root, rules);
    _checkRules(rules);

    _findFirst(rules);
    _findFollow(rules);

    _findPredictTable(rules);

    _genetateFile(out);
}

void TableGenerator::_genetateFile(std::ostream& out)
{
    out << "#pragma once \n#include <vector>\n#include <unordered_map>\n#include \"token.hpp\" \n\n";
    out << "enum NON_TERM : int \n{ \n\t// все терминалы имеют тип char, а значит значение < 256\n";
    int nterm_value = 256;
    for (const auto& left_nterm : leftNTerms) {
        out << "\tNON_TERM_" << left_nterm.attr << " =" << nterm_value << "," << std::endl;
        nterm_value++;
    }
    out << "\tROOT,\n};\n\n";

    out << "const NON_TERM Axiom = NON_TERM_" << axiom.attr << ";\n\n";

    out << "const std::unordered_map<int, const std::unordered_map<DOMAIN_TAG, std::vector<int>>> table {\n";
    for (const auto& row : predict_table) {
        out << "\t{ NON_TERM_" << row.first.attr << ",{\n";
        for (const auto& term : row.second) {
            out << "\t\t{";
            if (term.first.tag == 0) {
                out << 0;
            }
            else {
                out << "\'" << term.first.attr << "\'";
            }
            out << ", {";
            for (size_t i = 0; i < term.second.size(); ++i) {
                if (term.second[i].tag == 'i') {
                    out << "NON_TERM_" << term.second[i].attr;
                }
                else {
                    out << "\'" << term.second[i].attr << "\'";
                }
                if (i < term.second.size() - 1) {
                    out << ", ";
                }
            }
            out << "}},\n";
        }

        out << "\t}},\n";
    }

    out << "};\n";
}

void TableGenerator::_findFirst(const std::vector<Rule>& rules)
{
    bool is_change = true;
    while (is_change) {
        is_change = false;
        for (const auto& rule : rules) {
            for (size_t j = 0; j < rule.next.size(); ++j) {
                auto first_for_next = _findFirstForRule(rule.next[j]);
                size_t old_size = first[rule.nterm].size();
                first[rule.nterm].insert(first_for_next.begin(), first_for_next.end());
                if (first[rule.nterm].size() > old_size) {
                    is_change = true;
                }
            }
        }
    }
}

std::set<Token> TableGenerator::_findFirstForRule(const std::vector<Token>& tokens)
{
    std::set<Token> first_for_rule;
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i].tag == 's') {
            first_for_rule.insert(tokens[i]);
            return first_for_rule;
        }
        else {
            first_for_rule.insert(first[tokens[i]].begin(), first[tokens[i]].end());
            if (first[tokens[i]].find(EPSILON) != first[tokens[i]].end()) {
                first_for_rule.erase(EPSILON);
            }
            else {
                return first_for_rule;
            }
        }
    }
    first_for_rule.insert(EPSILON);
    return first_for_rule;
}

void TableGenerator::_findFollow(const std::vector<Rule>& rules)
{
    follow[axiom].insert(END);
    bool is_change = true;
    while (is_change) {
        is_change = false;
        for (const auto& rule : rules) {
            for (size_t j = 0; j < rule.next.size(); ++j) {
                std::vector<Token> right_part = rule.next[j];
                for (size_t i = 0; i < right_part.size(); ++i) {
                    if (right_part[i].tag == 'i') {
                        size_t old_size = follow[right_part[i]].size();

                        std::vector<Token> suffix(right_part.begin() + i + 1, right_part.end());
                        auto first_for_suffix = _findFirstForRule(suffix);
                        follow[right_part[i]].insert(first_for_suffix.begin(), first_for_suffix.end());
                        follow[right_part[i]].erase(EPSILON);

                        if (first_for_suffix.find(EPSILON) != first_for_suffix.end()) {
                            follow[right_part[i]].insert(
                                follow[rule.nterm].begin(), follow[rule.nterm].end());
                        }

                        if (old_size < follow[right_part[i]].size()) {
                            is_change = true;
                        }
                    }
                }
            }
        }
    }
}

void TableGenerator::_findPredictTable(const std::vector<Rule>& rules)
{
    for (const auto& rule : rules) {
        for (size_t i = 0; i < rule.next.size(); ++i) {
            auto first_for_rule = _findFirstForRule(rule.next[i]);
            if (first_for_rule.find(EPSILON) != first_for_rule.end()) {
                first_for_rule.insert(follow[rule.nterm].begin(), follow[rule.nterm].end());
            }
            first_for_rule.erase(EPSILON);

            for (const auto& term : first_for_rule) {
                if (predict_table[rule.nterm].find(term) != predict_table[rule.nterm].end()) {
                    throw std::runtime_error("Grammar is not LL(1)");
                }
                predict_table[rule.nterm][term] = rule.next[i];
            }
        }
    }
}

void TableGenerator::_checkRules(const std::vector<Rule>& rules)
{
    // Количество аксиом
    int cntAxioms = 0;
    for (size_t i = 0; i < rules.size(); ++i) {
        cntAxioms += rules[i].axiom;
        if (rules[i].axiom) {
            axiom = rules[i].nterm;
        }
    }
    if (cntAxioms != 1) {
        throw std::runtime_error("wrong number of axioms");
    }

    // наличие нетерминального символа, не присутствующего в левой части ни одного правила
    std::set<Token> unionNTerms = leftNTerms;
    unionNTerms.insert(rightNTerms.begin(), rightNTerms.end());
    if (leftNTerms.size() < unionNTerms.size()) {
        throw std::runtime_error("there is non terminal without a rule");
    }
}

void TableGenerator::_readRules(const std::shared_ptr<Node>& node, std::vector<Rule>& rules)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: Rules");
    }
    if (ntermnode->children.size() > 0) {
        rules.push_back(_readRule(ntermnode->children[0]));
        _readRules(ntermnode->children[1], rules);
    }
}

Rule TableGenerator::_readRule(const std::shared_ptr<Node>& node)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: Rule");
    }

    Token nterm = _readNTerm(ntermnode->children[1]);
    leftNTerms.insert(nterm);
    Rule rule{
        _readAxiom(ntermnode->children[0]),
        nterm,
        _readNTermRule(ntermnode->children[2])
    };
    return rule;
}

bool TableGenerator::_readAxiom(const std::shared_ptr<Node>& node)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: Axiom");
    }
    return ntermnode->children.size() == 1;
}

Token TableGenerator::_readNTerm(const std::shared_ptr<Node>& node)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: NTerm");
    }

    auto ident = std::dynamic_pointer_cast<LeafNode>(ntermnode->children[0]);
    if (!ident) {
        throw std::runtime_error("expected: LeafNode");
    }
    return ident->token;
}

std::vector<std::vector<Token>> TableGenerator::_readNTermRule(const std::shared_ptr<Node>& node)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: NTermRule");
    }
    std::vector<std::vector<Token>> argsOR;
    std::vector<Token> tokens;
    _readArgOR(ntermnode->children[1], tokens);
    argsOR.push_back(tokens);
    _readNTermRuleTail(ntermnode->children[3], argsOR);
    return argsOR;
}

std::vector<Token> TableGenerator::_readArgOR(const std::shared_ptr<Node>& node, std::vector<Token>& tokens)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: ArgOR");
    }
    if (ntermnode->children.size() > 0) {
        tokens.push_back(_readSymArgOR(ntermnode->children[0]));
        _readArgOR(ntermnode->children[1], tokens);
    }
    return tokens;
}



void TableGenerator::_readNTermRuleTail(const std::shared_ptr<Node>& node,
    std::vector<std::vector<Token>>& argsOR)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: NTermRuleTail");
    }
    if (ntermnode->children.size() > 0) {
        std::vector<Token> tokens;
        _readArgOR(ntermnode->children[1], tokens);
        argsOR.push_back(tokens);
        _readNTermRuleTail(ntermnode->children[3], argsOR);
    }
}

Token TableGenerator::_readSymArgOR(const std::shared_ptr<Node>& node)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: SymArgOR");
    }
    auto child = std::dynamic_pointer_cast<NTermNode>(ntermnode->children[0]);
    if (child) {
        Token token = _readNTerm(child);
        rightNTerms.insert(token);
        return token;
    }
    auto child2 = std::dynamic_pointer_cast<LeafNode>(ntermnode->children[0]);
    return child2->token;
}

```

Файл main.cpp
```cpp
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <memory>
#include <vector>

#include "analyzer/scanner.hpp"
#include "analyzer/parser.hpp"
#include "table_generator.hpp"

using namespace std;

int main(const int argc, const char **argv)
{
   if (argc < 3) {
      cout << "Wrong number of arguments!" << endl;
      return -1;
   }
   
   const string grammar_file = argv[1];
   const string table_dest_filename = argv[2];
   // const string grammar_file = "/home/ilya/projects/bmstu/compilers/lab8/grammar_test.txt";
   // const string table_dest_filename = 
   // "/home/ilya/projects/bmstu/compilers/lab8/grammar_language_table.hpp";

   ifstream cin(grammar_file);
   Scanner scanner(&cin, &cout);

   shared_ptr<semantic_type> lval = make_shared<semantic_type>();
   shared_ptr<location_type> frag = make_shared<location_type>();

   vector<Token> tokens;
   DOMAIN_TAG tag;
   do
   {
      tag = scanner.lex(lval, frag);
      Token new_token = Token(tag, *lval, *frag);
      tokens.push_back(new_token);
      cout << new_token << endl;
   } while (tag != 0);

   Parser p(tokens);
   shared_ptr<Node> root = p.parse();

   TableGenerator gen(root);
   ofstream table_out(table_dest_filename);
   gen.generate(table_out);

   // root->print(0);

   ofstream out("/home/ilya/projects/bmstu/compilers/lab8/tree.dot");
   root->printDOT(out);

   cout << endl;
   return 0;
}

```

## Calculator

Файл lexer.flex
```cpp
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

```

Файл calculator.hpp
```cpp
#pragma once 

#include "analyzer/node.hpp"

class Calculator {
public:
    Calculator(const std::shared_ptr<Node>& root): root(root) {}

    int calculate();
private:
    int _calcE(const std::shared_ptr<Node>& node);
    int _calcT(const std::shared_ptr<Node>& node);
    void _calcE_(int& num, const std::shared_ptr<Node>& node);
    void _calcT_(int& num, const std::shared_ptr<Node>& node);
    int _calcF(const std::shared_ptr<Node>& node);

    std::shared_ptr<Node> root = nullptr;
};

```

Файл calculator.cpp
```cpp
#include "calculator.hpp"

int Calculator::calculate()
{
    return _calcE(root);
}

int Calculator::_calcE(const std::shared_ptr<Node>& node)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: E");
    }
    int num = _calcT(ntermnode->children[0]);
    _calcE_(num, ntermnode->children[1]);
    return num;
}

int Calculator::_calcT(const std::shared_ptr<Node>& node)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: T");
    }
    int num = _calcF(ntermnode->children[0]);
    _calcT_(num, ntermnode->children[1]);
    return num;
}

void Calculator::_calcE_(int& num, const std::shared_ptr<Node>& node)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: E'");
    }
    if (ntermnode->children.size() > 0) {
        auto leafnode = std::dynamic_pointer_cast<LeafNode>(ntermnode->children[0]);
        int numT = _calcT(ntermnode->children[1]);
        if (leafnode->token.tag == '+') {
            num += numT;
        }
        _calcE_(num, ntermnode->children[2]);
    }
}

void Calculator::_calcT_(int& num, const std::shared_ptr<Node>& node)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: T'");
    }
    if (ntermnode->children.size() > 0) {
        auto leafnode = std::dynamic_pointer_cast<LeafNode>(ntermnode->children[0]);
        int numT = _calcF(ntermnode->children[1]);
        if (leafnode->token.tag == '*') {
            num *= numT;
        }
        _calcT_(num, ntermnode->children[2]);
    }
}

int Calculator::_calcF(const std::shared_ptr<Node>& node)
{
    auto ntermnode = std::dynamic_pointer_cast<NTermNode>(node);
    if (!ntermnode) {
        throw std::runtime_error("expected: F");
    }

    if (ntermnode->children.size() == 1) {
        auto leafnode = std::dynamic_pointer_cast<LeafNode>(ntermnode->children[0]);
        int num = atoi(leafnode->token.attr.c_str());
        return num;
    } else {
        return _calcE(ntermnode->children[1]);
    }
    return 0;
}

```

Файл main.cpp
```cpp
#include <iostream>
#include <fstream>

#include "analyzer/scanner.hpp"
#include "calculator.hpp"

using namespace std;

int main(const int argc, const char** argv)
{
    if (argc < 2) {
        cout << "Wrong number of arguments!" << endl;
        return -1;
    }
    
    const string grammar_file = argv[1];
    // const string grammar_file = "/home/ilya/projects/bmstu/compilers/lab8/arithexpr_test.txt";

    ifstream cin(grammar_file);
    Scanner scanner(&cin, &cout);

    shared_ptr<semantic_type> lval = make_shared<semantic_type>();
    shared_ptr<location_type> frag = make_shared<location_type>();

    vector<Token> tokens;
    DOMAIN_TAG tag;
    do
    {
        tag = scanner.lex(lval, frag);
        Token new_token = Token(tag, *lval, *frag);
        tokens.push_back(new_token);
        cout << new_token << endl;
    } while (tag != 0);

    Parser p(tokens);
    shared_ptr<Node> root = p.parse();
    ofstream out("/home/ilya/projects/bmstu/compilers/lab8/tree.dot");
    root->printDOT(out);

    Calculator calc(root);

    cout << endl;
    cout << calc.calculate();

    cout << endl;
    return 0;
}

```

# Тестирование

## Generator
Входные данные

```
/* аксиома помечена звёздочкой */
  F  ("n") ("(" E ")")
  T  (F T')
  T' ("*" F T') ()
* E  (T E')
  E' ("+" T E') ()

```

Вывод на `stdout`

```cpp
#pragma once 
#include <vector>
#include <unordered_map>
#include "token.hpp" 

enum NON_TERM : int 
{ 
	// все терминалы имеют тип char, а значит значение < 256
	NON_TERM_E =256,
	NON_TERM_E_ =257,
	NON_TERM_F =258,
	NON_TERM_T =259,
	NON_TERM_T_ =260,
	ROOT,
};

const NON_TERM Axiom = NON_TERM_E;

const std::unordered_map<int, const std::unordered_map<DOMAIN_TAG, std::vector<int>>> table {
	{ NON_TERM_T_,{
		{'+', {}},
		{')', {}},
		{0, {}},
		{'*', {'*', NON_TERM_F, NON_TERM_T_}},
	}},
	{ NON_TERM_E,{
		{'n', {NON_TERM_T, NON_TERM_E_}},
		{'(', {NON_TERM_T, NON_TERM_E_}},
	}},
	{ NON_TERM_T,{
		{'n', {NON_TERM_F, NON_TERM_T_}},
		{'(', {NON_TERM_F, NON_TERM_T_}},
	}},
	{ NON_TERM_E_,{
		{')', {}},
		{0, {}},
		{'+', {'+', NON_TERM_T, NON_TERM_E_}},
	}},
	{ NON_TERM_F,{
		{'(', {'(', NON_TERM_E, ')'}},
		{'n', {'n'}},
	}},
};

```

## Calculator
Входные данные

```
1 + 2 * (3 + 1)
```

Вывод на `stdout`

```
9
```

# Вывод
Получил искреннее удовольствие в написании данной лабораторной. 
Получил опыт в написании самоприменимого генератора таблиц. 
Сначала было очень трудно разобраться, в чем заключается идея лабораторной и, казалось,
 что лабораторная очень запутанная. 
 Я для себя написал инструкцию к тому, в какой последовательности должны 
 работать компоненты и какие данные они должны принимать на входе и какие генерировать на выходе. 
 Генерация таблицы производится интересным способом на языке C++. 
 Вспомнил, как подключать библиотеки и создавать несколько исполняемых целей с помощью cmake.
 Также получил опыт в анализе дерева вывода и извлечении полезной информации из него 
 (при построении правил грамматики или в подсчете арифметического выражения). 
