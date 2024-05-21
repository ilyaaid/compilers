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
