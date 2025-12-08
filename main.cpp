#include "lexer.h"
#include "parser.h"

#include <iostream>

int main(int argc, char const *argv[])
{
  std::string input; 
  std::getline(std::cin, input);

  Lexer lexer(input);
  try
  {
    Parser parser(lexer.get_tokens());
    std::cout << parser.evaluate() << "\n";
  }
  catch(const std::runtime_error& e)
  {
    std::cerr << e.what() << '\n';
  }
  
  return 0;
}
