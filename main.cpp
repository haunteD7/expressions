#include "lexer.h"
#include "parser.h"
#include "AST_printer.h"

#include <iostream>

int main(int argc, char const *argv[])
{
  std::string input; 
  std::getline(std::cin, input);

  Lexer lexer(input);
  try
  {
    Parser parser(lexer.get_tokens());

    const Expr& ast = parser.get_ast();

    std::cout << "Infix:    " << to_infix(ast) << "\n";
    std::cout << "Prefix:   " << to_prefix(ast) << "\n";
    std::cout << "Postfix:  " << to_postfix(ast) << "\n";
    std::cout << "\nExpressionTree\n";
    print_tree(ast);    
    std::cout << "\nResult:" << parser.evaluate() << "\n";
  }
  catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
  }
  
  return 0;
}
