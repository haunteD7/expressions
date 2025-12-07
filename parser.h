#pragma once

#include "AST.h"
#include "lexer.h"

class Parser
{
public:
  Parser(const std::vector<Token>& tokens) : tokens(tokens)
  {

  }
  void parse() {
    token_pos = 0;
  }
private:

  const std::vector<Token>& tokens;
  size_t token_pos = 0;
  std::shared_ptr<ASTNode> ast;
};