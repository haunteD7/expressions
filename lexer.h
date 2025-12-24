#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <iostream>

enum class TokenType 
{
  Plus,
  Minus,
  Asterix,
  Slash,
  Caret,
  Exclaim,

  LeftParen,
  RightParen,
  Eof,

  Number,
};
struct Token
{
  TokenType type;
  double value;
};

class Lexer
{
public:
  Lexer(const std::string_view text) : text(text), text_pos(0)
  {
    lexify();
  }

  void lexify() {
    tokens.clear();
    text_pos = 0;

    size_t text_size = text.size();
    while(text_pos < text_size) {
      char symbol = peek();
      if(std::isspace(symbol)) {
        ++text_pos;
        continue;
      }
      if(std::isdigit(symbol)) {
        tokens.push_back(tokenize_number());
        continue;
      }
      switch (symbol)
      {
        case '+': {
          tokens.push_back({TokenType::Plus});
          break;
        }
        case '-': {
          tokens.push_back({TokenType::Minus});
          break;
        }
        case '*': {
          tokens.push_back({TokenType::Asterix});
          break;
        }
        case '/': {
          tokens.push_back({TokenType::Slash});
          break;
        }
        case '^': {
          tokens.push_back({TokenType::Caret});
          break;
        }
        case '!': {
          tokens.push_back({TokenType::Exclaim});
          break;
        }
        case '(': {
          tokens.push_back({TokenType::LeftParen});
          break;
        }
        case ')': {
          tokens.push_back({TokenType::RightParen});
          break;
        }
        default: {
          throw std::runtime_error(std::format("Illegal symbol: '{}'", symbol));
          break;
        }
      }
      ++text_pos;
    }
    tokens.push_back({TokenType::Eof});
  }
  Token tokenize_number() {
    size_t text_size = text.size();

    size_t number_substr_start = text_pos;
    while(text_pos < text_size && std::isdigit(peek())) {
      ++text_pos;
    }

    std::string_view number_substr = text.substr(number_substr_start, text_pos);
    double result;
    std::from_chars(number_substr.data(), number_substr.data() + number_substr.size(), result);

    return { TokenType::Number, result };
  }

  static const char* get_token_type_str(TokenType type) {
    switch (type)
    {
      case TokenType::Plus:
        return "+";
      case TokenType::Minus:
        return "-";
      case TokenType::Asterix:
        return "*";
      case TokenType::Slash:
        return "/";
      case TokenType::Caret:
        return "^";
      case TokenType::Exclaim:
        return "!";

      case TokenType::LeftParen:
        return "(";
      case TokenType::RightParen:
        return ")";

      case TokenType::Number:
        return "Number";
    }
    return "Unknown";
  }

  const auto& get_tokens() const { return tokens; }
private:
  char peek() {
    return text[text_pos];
  }
  
  std::string_view text;
  std::vector<Token> tokens;

  size_t text_pos;
};