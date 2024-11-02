#pragma once

#include <iostream>
#include <vector>
#include <optional>


struct NodeExpr{
    Token int_lit;
};

struct NodeReturn{
    NodeExpr expression;
};


class Parser{

public:

    Parser(std::vector<Token> tokens)
    : m_tokens(tokens)
    {

    }

    std::optional<NodeExpr> ParseExpr(){
        if(peek().has_value() && peek().value().type == TokenType::INT_LIT){
            NodeExpr expr = {consume()};
            return expr;
        } else{
            return std::nullopt;
        }
    }

    std::optional<NodeReturn> parse(){
        while(peek().has_value()){
            if(peek().value().type == TokenType::_return){
                consume();

                std::optional<NodeExpr> expr = ParseExpr();
                if(expr){
                    return NodeReturn {expr.value()};
                } else{
                    std::cerr << "Invlaid Expression" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

private:

    int m_index;
    std::vector<Token> m_tokens;

    [[nodiscard]] std::optional<Token> peek(int offset = 0){
        if(m_index + offset >= m_tokens.size()){
            return std::nullopt;
        } else{
            return m_tokens[m_index + offset];
        }
    }

    Token consume(){
        return m_tokens[m_index++];
    }
};