#pragma once

#include <stdexcept>


#include "AST.h"
#include "lexer.h"

class Parser
{
public:
  Parser(const std::vector<Token>& tokens) : tokens(tokens) {
    parse(); 
  }
  void parse() {
    token_pos = 0;
    AST = parse_expression();
  }
  double evaluate() {
    return evaluate_AST(AST.get());
  }
  const Expr& get_ast() const {
    return *AST;
  }
private:
  std::unique_ptr<Expr> parse_expression() {
    return parse_factor();
  }
  std::unique_ptr<Expr> parse_factor() {
    auto result = parse_term();

    while(true) {
      TokenType type = peek().type;
      if(type == TokenType::Plus) {
        advance();
        result = std::make_unique<Expr>(BinaryExpr{std::move(result), parse_term(), Operation::Add});
      }
      else if(type == TokenType::Minus) {
        advance();
        result = std::make_unique<Expr>(BinaryExpr{std::move(result), parse_term(), Operation::Substract});
      }
      else break;
    }

    return result;
  }
  std::unique_ptr<Expr> parse_term() {
    auto result = parse_unary();
    Operation op;

    while(true) {
      TokenType type = peek().type;
      if(type == TokenType::Asterix) {
        op = Operation::Multipy;
      }
      else if(type == TokenType::Slash) {
        op = Operation::Divide;
      }
      else break;

      advance();
      result = std::make_unique<Expr>(BinaryExpr{std::move(result), parse_unary(), op});
    }

    return result;
  }
  std::unique_ptr<Expr> parse_unary() {
    TokenType type = peek().type;
    if(type == TokenType::Minus) {
      advance();
      return std::make_unique<Expr>(UnaryExpr{parse_unary(), Operation::Negate});
    }
    else {
      return parse_power();
    }
  }
  std::unique_ptr<Expr> parse_power() {
    TokenType type = peek().type;

    auto result = parse_primary();
    while(peek().type == TokenType::Caret) {
      advance();
      result = std::make_unique<Expr>(BinaryExpr{std::move(result), parse_primary(), Operation::Power});
    }
  
    return result;
  }
  std::unique_ptr<Expr> parse_primary() {
    TokenType type = peek().type;
    if(type == TokenType::Number) {
      auto result = std::make_unique<Expr>(NumberExpr{peek().value});
      advance();
      return result;
    }
    else if(type == TokenType::LeftParen) {
      advance();
      auto result = parse_expression();
      if(!match(TokenType::RightParen)) {
        throw std::runtime_error("Expected )");
      }

      return result;
    }
    else throw std::runtime_error("Unexpected token: " + std::string(Lexer::get_token_type_str(peek().type)));
  } 

  bool match(TokenType type) {
    if(check(type)) {
      advance();
      return true;
    }
    return false;
  }
  void advance() {
    if(!check(TokenType::Eof)) ++token_pos;
  }
  void back() {
    if(token_pos > 0) --token_pos;
  }
  bool check(TokenType type) const { 
    return peek().type == type; 
  }
  const Token& peek() const {
    return tokens[token_pos];
  }

  const std::vector<Token>& tokens;
  size_t token_pos = 0;
  std::unique_ptr<Expr> AST;
};