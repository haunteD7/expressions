#pragma once

#include <memory>
#include <variant>
#include <cstdint>
#include <cmath>

static double factorial(double v) {
  if(v == 0) return 1;

  return v * factorial(v - 1);
}

enum class Operation
{
  Add, 
  Substract,
  Negate,
  Multipy,
  Divide,
  Power,
  Factorial,
};

struct BinaryExpr;
struct UnaryExpr;
struct NumberExpr;
using Expr = std::variant<
  BinaryExpr,
  UnaryExpr,
  NumberExpr
>;
struct BinaryExpr
{
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;
  Operation op;
};   
struct UnaryExpr
{
  std::unique_ptr<Expr> expr;
  Operation op;
};
struct NumberExpr
{
  double number;
};

static double evaluate_AST(Expr* expr) {
  if(std::holds_alternative<BinaryExpr>(*expr)) {
    auto& binary_expr = std::get<BinaryExpr>(*expr);
    switch (binary_expr.op)
    {
      case Operation::Add:
        return evaluate_AST(binary_expr.left.get()) + evaluate_AST(binary_expr.right.get());
      case Operation::Substract:
        return evaluate_AST(binary_expr.left.get()) - evaluate_AST(binary_expr.right.get());
      case Operation::Multipy:
        return evaluate_AST(binary_expr.left.get()) * evaluate_AST(binary_expr.right.get());
      case Operation::Divide:
      {
        double right = evaluate_AST(binary_expr.right.get());
        if(right == 0) throw std::runtime_error("Division by zero");
        return evaluate_AST(binary_expr.left.get()) / right;
      }
      case Operation::Power:
        return std::pow(evaluate_AST(binary_expr.left.get()), evaluate_AST(binary_expr.right.get()));
    }
  }
  else if(std::holds_alternative<UnaryExpr>(*expr)) {
    auto& unary_expr = std::get<UnaryExpr>(*expr);
    switch (unary_expr.op)
    {
      case Operation::Negate:
        return -evaluate_AST(unary_expr.expr.get());
      case Operation::Factorial:
        double val = evaluate_AST(unary_expr.expr.get());
        if(val < 0) throw std::runtime_error("Negative factorial");

        return factorial(val);
    }
  }
  else if(std::holds_alternative<NumberExpr>(*expr)) {
    auto& number_expr = std::get<NumberExpr>(*expr);
    return number_expr.number; 
  }

  return 0;
}