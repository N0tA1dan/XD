#include <iostream>
#include <fstream>
#include <optional>
#include <vector>

enum class TokenType{
  _exit,
  INT_LIT,
  DOUBLE_QUOTE,
  SEMI
};

struct Token{
  TokenType type;
  std::optional<std::string> value;
};

class Tokenizer{
private:
    std::string m_src;
    int m_index = 0;
    
    [[nodiscard]] std::optional<char> peek(int offset  = 0){
        if(m_index + offset >= m_src.length()){
            return std::nullopt;
        } else{
            return m_src.at(m_index + offset);
        }
    }

    char consume(){
        return m_src.at(m_index++);
    }

public:
    Tokenizer(std::string src)
    : m_src(src)
    {

    }

    std::vector<Token> tokenize(){
        std::string buf;
        std::vector<Token> tokens;

        while(peek().has_value()){
            if(std::isalpha(peek().value())){
                buf.push_back(consume());
                while(peek().has_value() && std::isalnum(peek().value())){
                    buf.push_back(consume());
                    continue;
                }

                if(buf == "exit"){
                    tokens.push_back({TokenType::_exit});
                    buf.clear();
                    continue;
                }
                continue;
            }

            if(std::isdigit(peek().value())){
                buf.push_back(consume());
                while(peek().has_value() && std::isdigit(peek().value())){
                    buf.push_back(consume());
                }
                
                tokens.push_back({TokenType::INT_LIT, buf});
                buf.clear();
            }

            if(peek().value() == ';'){
                tokens.push_back({TokenType::SEMI});
                consume();
                continue;
            }

            if(std::isspace(peek().value())){
                consume();
                continue;
            }

            // make sure to increase the index
            consume();
        }


        m_index = 0;
        return tokens;
    }


};


int main(int argc, char * argv[]){
  
  std::ifstream inputFile(argv[1]);
  if (!inputFile) {
      std::cerr << "Could not open file: " << argv[1] << std::endl;
      return 1;
  }

  std::string code((std::istreambuf_iterator<char>(inputFile)),
                   std::istreambuf_iterator<char>());

  Tokenizer tokenizer(code);
  std::vector<Token> tokens = tokenizer.tokenize();

  for (const auto& token : tokens) {
      switch (token.type) {
          case TokenType::_exit: std::cout << "exit "; break;
          case TokenType::INT_LIT: std::cout << *token.value << " "; break;
          case TokenType::SEMI: std::cout << "; "; break;
          default: break;
      }
  }

}