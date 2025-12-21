#include "lexer.h"
#include "parser.h"
#include "AST_printer.h"

#include <iostream>

int main(int argc, char const *argv[])
{
  std::string input; 
  std::getline(std::cin, input);

  try
  {
    Lexer lexer(input);
    Parser parser(lexer.get_tokens());
    double result = parser.evaluate();

    const Expr& ast = parser.get_ast();

    std::cout << "Infix:    " << to_infix(ast) << "\n";
    std::cout << "Prefix:   " << to_prefix(ast) << "\n";
    std::cout << "Postfix:  " << to_postfix(ast) << "\n";
    std::cout << "\nExpressionTree\n";
    print_tree(ast);    
    std::cout << "\nResult:" << result << "\n";
  }
  catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
  }
  
  return 0;
}
