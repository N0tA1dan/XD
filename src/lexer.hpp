#include <iostream>
#include <fstream>
#include <optional>
#include <vector>

enum class TokenType{
  _return,
  LET,
  IDENTIFIER,
  EQUALS,
  INT_LIT,
  STRING_LIT,
  PLUS,
  MINUS,
  MULTIPLY,
  DIVIDE,
  SEMI,
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

            // handles keywords
            if(std::isalpha(peek().value())){
                buf.push_back(consume());
                while(peek().has_value() && std::isalnum(peek().value())){
                    buf.push_back(consume());
                    continue;
                }

                if(buf == "return"){
                    tokens.push_back({TokenType::_return});
                    buf.clear();
                    continue;
                }

                if(buf == "let"){
                    tokens.push_back({TokenType::LET});
                    buf.clear();
                    continue;
                }

                else{
                    tokens.push_back({TokenType::IDENTIFIER, buf});
                    buf.clear();
                    continue;
                }

                continue;
            }
            
            // Handles integer literals
            if(std::isdigit(peek().value())){
                buf.push_back(consume());
                while(peek().has_value() && std::isdigit(peek().value())){
                    buf.push_back(consume());
                }
                
                tokens.push_back({TokenType::INT_LIT, buf});
                buf.clear();
            }

            // Handles string literals
            if(peek().value() == '"'){
                consume();
                while(peek().has_value() && peek().value() != '"'){
                    buf.push_back(consume());
                }

                if(peek().has_value() && peek().value() == '"'){
                    consume();
                    tokens.push_back({TokenType::STRING_LIT, buf});
                    buf.clear();
                }
            }

            if(peek().value() == '='){
                tokens.push_back({TokenType::EQUALS});
                consume();
                continue;
            }

            if(peek().value() == '+'){
                tokens.push_back({TokenType::PLUS});
                consume();
                continue;
            }

            if(peek().value() == '-'){
                tokens.push_back({TokenType::MINUS});
                consume();
                continue;
            }

            if(peek().value() == '*'){
                tokens.push_back({TokenType::MULTIPLY});
                consume();
                continue;
            }

            if(peek().value() == '/'){
                tokens.push_back({TokenType::DIVIDE});
                consume();
                continue;
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
