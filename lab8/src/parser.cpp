#include "parser.hpp"
#include "node.hpp"
#include "token.hpp"

#include <tuple>
#include <stack>

Parser::Parser(const std::vector<Token> &tokens)
{
    this->tokens = tokens;
}

std::shared_ptr<Node> Parser::parse()
{
    std::stack<ParseNode> stack;
    stack.push({DOMAIN_TAG::TAG_END, nullptr});

    auto root = std::make_shared<NTermNode>(NON_TERM::ROOT);
    stack.push({NON_TERM::Rules, root});

    int token_ind = 0;
    while (!stack.empty()) {
        ParseNode cur = stack.top();
        stack.pop();

        if (cur.tagnterm.is_tag) {
            if (cur.tagnterm.tag == tokens[token_ind].tag) {
                if (cur.tagnterm.tag != DOMAIN_TAG::TAG_END) {
                    auto new_child = std::make_shared<LeafNode>(tokens[token_ind]);
                    cur.node->children.push_back(new_child);
                }
                token_ind++;
            } else {
                throw std::runtime_error("wrong tag: \n\treceived:" + Token::tag2string[tokens[token_ind].tag] + "\n\texpected:" + Token::tag2string[cur.tagnterm.tag]);
            }
        } else {
            std::vector<TagORNTerm> next_rule = table[cur.tagnterm.nterm][tokens[token_ind].tag];
            if (next_rule.size() > 0) {
                auto new_parent = std::make_shared<NTermNode>(cur.tagnterm.nterm);
                cur.node->children.push_back(new_parent);
                for (int i = next_rule.size() - 1; i >= 0; i--) {
                    stack.push({next_rule[i], new_parent});
                }
            }
        }
    }
    return root->children[0];
}
