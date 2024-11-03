#pragma once

#include <iostream>
#include <vector>
#include <optional>
#include <variant>

struct NodeExpr{
    Token var;
};

struct NodeStmtReturn{
    NodeExpr expression;
};

struct NodeStmtLet{
    Token identifier;
    NodeExpr expression;
};


struct NodeStmt{
    std::variant<NodeStmtReturn, NodeStmtLet> var;
};

struct NodeProg{
    std::vector<NodeStmt> stmts;
};


class Parser{

public:

    Parser(std::vector<Token> tokens)
    : m_tokens(tokens)
    {

    }
    
    std::optional<NodeExpr> ParseExpr(){
        if(peek().has_value() && (peek().value().type == TokenType::INT_LIT || peek().value().type == TokenType::STRING_LIT)){
            NodeExpr expr = {consume()};
            return expr;
        } else{
            return std::nullopt;
        }
    }

    std::optional<NodeStmt> ParseStmt() {

        if (peek().value().type == TokenType::_return) {
            consume(); // consume return token

            NodeStmtReturn returnstmt;

            auto expr = ParseExpr();
            if (expr) {
                returnstmt.expression = expr.value();
            } else {
                std::cerr << "Invalid Expression" << std::endl;
                exit(EXIT_FAILURE);
            }
            
            if(peek().has_value() && peek().value().type == TokenType::SEMI){
                consume();
            } else{
                std::cerr << "Expected ';' after return statement";
                exit(EXIT_FAILURE);
            }
            
            return NodeStmt { returnstmt };
        }

        if (peek().value().type == TokenType::LET) {
            consume(); // consume let token

            NodeStmtLet let;
            let.identifier = consume(); // consume identifier

            // Check for '=' token
            if (peek().has_value() && peek().value().type == TokenType::EQUALS) {
                consume(); // consume '=' token
            } else {
                std::cerr << "Expected '=' after identifier in let statement" << std::endl;
                exit(EXIT_FAILURE);
            }

            auto expression = ParseExpr();

            if (expression) {
                let.expression = expression.value();

            } else {
                std::cerr << "Error parsing expression in let statement" << std::endl;
                exit(EXIT_FAILURE);
            }

            if(peek().has_value() && peek().value().type == TokenType::SEMI){
                consume();
            } else{
                std::cerr << "Expected ';' after expression in let statement";
                exit(EXIT_FAILURE);
            }

            return NodeStmt { let };
        }

        // If no matching statement, return null optional
        return std::nullopt;
    }    

    std::optional<NodeProg> ParseProg()
    {
        NodeProg prog;
        while (peek().has_value()) {
            if (auto stmt = ParseStmt()) {
                prog.stmts.push_back(stmt.value());
            }
            else {
                std::cerr << "expected statement" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        return prog;
    }

private:

    int m_index = 0;
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