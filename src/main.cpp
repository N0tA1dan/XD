#include <iostream>
#include <fstream>
#include <optional>
#include <vector>

enum class TokenType{
  _return,
  INT_LIT,
  DOUBLE_QUOTE,
  SEMI
};

struct Token{
  TokenType type;
  std::optional<std::string> value;
};

std::vector<Token> tokenize(std::string str){
  std::string buf;
  std::vector<Token> tokens;

  for(int i = 0; i<str.size(); i++){
    
    // anything that has words like return or even strings. more or less like keywords.
    if(std::isalpha(str.at(i))){
      buf.push_back(str.at(i));
      i++;
      while(i<str.size() && isalnum(str.at(i))){
        buf.push_back(str.at(i));
        i++;
      }
      i--;

      // handle different cases here
      if(buf == "return"){
        tokens.push_back({.type = TokenType::_return});
        buf.clear();
        continue;
      }
    }

    // handles integers
    if(std::isdigit(str.at(i))){
      buf.push_back(str.at(i));
      i++;
      while(i<str.size() && std::isdigit(str.at(i))){
        buf.push_back(str.at(i));
        i++;
      }
      i--;
      tokens.push_back({.type = TokenType::INT_LIT, .value = buf});
      buf.clear();
      continue;
    }

    // handles semi colons
    if(str.at(i) == ';'){
      tokens.push_back({.type = TokenType::SEMI});
      buf.clear();
      continue;
    }

    if(std::isspace(str.at(i))){
      continue;
    }
  }

  return tokens;
  
}

int main(int argc, char * argv[]){
  
  std::ifstream inputFile(argv[1]);
  if (!inputFile) {
      std::cerr << "Could not open file: " << argv[1] << std::endl;
      return 1;
  }

  std::string code((std::istreambuf_iterator<char>(inputFile)),
                   std::istreambuf_iterator<char>());
  std::vector<Token> tokens = tokenize(code);

  for (const auto& token : tokens) {
      switch (token.type) {
          case TokenType::_return: std::cout << "return "; break;
          case TokenType::INT_LIT: std::cout << *token.value << " "; break;
          case TokenType::SEMI: std::cout << "; "; break;
          default: break;
      }
  }

}