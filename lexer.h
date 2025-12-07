#pragma once

#include <string>
#include <cstdint>
#include <charconv>
#include <vector>
#include <variant>

enum class TokenType 
{
  Unknown,
  Plus,
  Minus,
  Asterix,
  Slash,
  Caret,

  LeftParen,
  RightParen,

  Number,
};
struct TokenOperator
{
  TokenType type;
};
struct TokenPunctuator
{
  TokenType type;
};
struct TokenNumber 
{
  TokenType type = TokenType::Number;
  std::uint64_t value;
};
using Token = std::variant<
  TokenNumber,
  TokenOperator,
  TokenPunctuator
>;

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
          tokens.push_back(TokenOperator({TokenType::Plus}));
          break;
        }
        case '-': {
          tokens.push_back(TokenOperator({TokenType::Minus}));
          break;
        }
        case '*': {
          tokens.push_back(TokenOperator({TokenType::Asterix}));
          break;
        }
        case '/': {
          tokens.push_back(TokenOperator({TokenType::Slash}));
          break;
        }
        case '^': {
          tokens.push_back(TokenOperator({TokenType::Caret}));
          break;
        }
        case '(': {
          tokens.push_back(TokenPunctuator({TokenType::LeftParen}));
          break;
        }
        case ')': {
          tokens.push_back(TokenPunctuator({TokenType::RightParen}));
          break;
        }
      }
      ++text_pos;
    }
  }
  TokenNumber tokenize_number() {
    size_t text_size = text.size();

    size_t number_substr_start = text_pos;
    while(text_pos < text_size && std::isdigit(peek())) {
      ++text_pos;
    }

    std::string_view number_substr = text.substr(number_substr_start, text_pos);
    std::uint64_t result;
    std::from_chars(number_substr.data(), number_substr.data() + number_substr.size(), result);

    return { TokenType::Number, result };
  }

  static const char* get_token_type_str(TokenType type) {
    switch (type)
    {
      case TokenType::Unknown:
        return "Unknown";
      case TokenType::Plus:
        return "Plus";
      case TokenType::Minus:
        return "Minus";
      case TokenType::Asterix:
        return "Asterix";
      case TokenType::Slash:
        return "Slash";
      case TokenType::Caret:
        return "Caret";

      case TokenType::LeftParen:
        return "LeftParen";
      case TokenType::RightParen:
        return "RightParen";

      case TokenType::Number:
        return "Number";
    }
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