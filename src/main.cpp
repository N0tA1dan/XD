#include <iostream>
#include <fstream>
#include <optional>
#include <vector>

#include "lexer.hpp"
#include "parser.hpp"

int main(int argc, char * argv[]){
  
  std::ifstream inputFile(argv[1]);
  if (!inputFile) {
      std::cerr << "Could not open file: " << argv[1] << std::endl;
      return 1;
  }


  std::string code((std::istreambuf_iterator<char>(inputFile)),
                   std::istreambuf_iterator<char>());
  
  Tokenizer tokenize(code);  
  std::vector<Token> tokens = tokenize.tokenize();

  Parser parser(tokens);
  parser.parse();

  for (const auto& token : tokens) {
      switch (token.type) {
          case TokenType::_return: std::cout << "return "; break;
          case TokenType::INT_LIT: std::cout << *token.value << " "; break;
          case TokenType::SEMI: std::cout << "; "; break;
          case TokenType::STRING_LIT: std::cout << "STRING_LIT value: " << *token.value << std::endl; break;
          case TokenType::PLUS: std::cout << "PLUS_TOKEN" << std::endl; break;
          default: break;
      }
  }

}