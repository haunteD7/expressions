#pragma once

#include "AST.h"
#include <string>
#include <iostream>
#include <sstream>

//----------------------------------
// Вспомогательное: оператор -> строка
//----------------------------------
inline std::string op_to_string(Operation op) {
  switch (op) {
    case Operation::Add:       return "+";
    case Operation::Substract: return "-";
    case Operation::Multipy:   return "*";
    case Operation::Divide:    return "/";
    case Operation::Power:     return "^";
    case Operation::Negate:    return "-";
  }
  return "?";
}

//----------------------------------
// Инфиксная форма
//----------------------------------
inline std::string to_infix(const Expr& expr) {
  if (std::holds_alternative<NumberExpr>(expr)) {
    return std::to_string(std::get<NumberExpr>(expr).number);
  }

  if (std::holds_alternative<UnaryExpr>(expr)) {
    const auto& u = std::get<UnaryExpr>(expr);
    return "(-" + to_infix(*u.expr) + ")";
  }

  const auto& b = std::get<BinaryExpr>(expr);
  return "(" +
         to_infix(*b.left) + " " +
         op_to_string(b.op) + " " +
         to_infix(*b.right) +
         ")";
}

//----------------------------------
// Префиксная форма
//----------------------------------
inline std::string to_prefix(const Expr& expr) {
  if (std::holds_alternative<NumberExpr>(expr)) {
    return std::to_string(std::get<NumberExpr>(expr).number);
  }

  if (std::holds_alternative<UnaryExpr>(expr)) {
    const auto& u = std::get<UnaryExpr>(expr);
    return "-" + to_prefix(*u.expr);
  }

  const auto& b = std::get<BinaryExpr>(expr);
  return op_to_string(b.op) + " " +
         to_prefix(*b.left) + " " +
         to_prefix(*b.right);
}

//----------------------------------
// Постфиксная форма
//----------------------------------
inline std::string to_postfix(const Expr& expr) {
  if (std::holds_alternative<NumberExpr>(expr)) {
    return std::to_string(std::get<NumberExpr>(expr).number);
  }

  if (std::holds_alternative<UnaryExpr>(expr)) {
    const auto& u = std::get<UnaryExpr>(expr);
    return to_postfix(*u.expr) + " -";
  }

  const auto& b = std::get<BinaryExpr>(expr);
  return to_postfix(*b.left) + " " +
         to_postfix(*b.right) + " " +
         op_to_string(b.op);
}

//----------------------------------
// Текстовая визуализация дерева
//----------------------------------
inline void print_tree(const Expr& expr, int indent = 0) {
  std::cout << std::string(indent, ' ');

  if (std::holds_alternative<NumberExpr>(expr)) {
    std::cout << std::get<NumberExpr>(expr).number << "\n";
    return;
  }

  if (std::holds_alternative<UnaryExpr>(expr)) {
    std::cout << "NEG\n";
    print_tree(*std::get<UnaryExpr>(expr).expr, indent + 2);
    return;
  }

  const auto& b = std::get<BinaryExpr>(expr);
  std::cout << op_to_string(b.op) << "\n";
  print_tree(*b.left, indent + 2);
  print_tree(*b.right, indent + 2);
}
