#pragma once

#include <memory>

enum class Operation
{
  Add, 
  Substract,
  Negate,
  Multipy,
  Divide,
  Power,
};

struct ASTNode
{
  std::shared_ptr<ASTNode> left;
  std::shared_ptr<ASTNode> right; 
  Operation op;
};