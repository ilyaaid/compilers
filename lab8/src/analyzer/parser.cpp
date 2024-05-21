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
