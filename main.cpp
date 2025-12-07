#include "lexer.h"

#include <iostream>

int main(int argc, char const *argv[])
{
  std::string input; 
  std::getline(std::cin, input);

  Lexer lexer(input);
  return 0;
}
