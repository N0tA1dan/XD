#pragma once

#include <iostream>
#include <vector>
#include <optional>
#include <variant>
#include <string>
#include <sstream>

struct NodeIntLit{
    Token int_lit;
};

struct NodeIdent{
    Token ident;
};

struct NodeExpr{
    std::variant<NodeIntLit, NodeIdent> var;
};

struct NodeStmtExit{
    NodeExpr expression;
};

struct NodeStmtLet{
    Token identifier;
    NodeExpr expression;
};

struct NodeStmt{
    std::variant<NodeStmtExit, NodeStmtLet> var;
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

    void tryConsume(TokenType token){
        if(peek().has_value() && peek().value().type == token){
            consume();
        } else{
            switch(token){
                case TokenType::SEMI:
                    std::cerr << "Expected ';' after statement";
                    break;

                case TokenType::OPEN_PAREN:
                    std::cerr << "Expected '(' before expression";
                    break;
                
                case TokenType::CLOSE_PAREN:
                    std::cerr << "Expected ')' after expression";
                    break;
            }

            exit(EXIT_FAILURE);
        }
    }
    
    std::optional<NodeExpr> ParseExpr(){
        NodeExpr expr;
        switch(peek().value().type){
            case TokenType::INT_LIT:
                {
                NodeIntLit int_lit = {consume()};
                expr.var = int_lit;
                return expr;
                break;
                }

            case TokenType::IDENTIFIER:
                {
                NodeIdent identifier = {consume()};
                expr.var = identifier;
                return expr;
                break;
                }

            default:
                {
                return std::nullopt;
                }

        }
    }

    std::optional<NodeStmt> ParseStmt() {


        // parses exit statements
        if (peek().value().type == TokenType::EXIT) {
            consume(); // consume exit token

            NodeStmtExit exitstmt;
            tryConsume(TokenType::OPEN_PAREN);

            auto expr = ParseExpr();
            if (expr) {
                exitstmt.expression = expr.value();
            } else {
                std::cerr << "Invalid Expression" << std::endl;
                exit(EXIT_FAILURE);
            }
            
            tryConsume(TokenType::CLOSE_PAREN);
            tryConsume(TokenType::SEMI);
            
            return NodeStmt { exitstmt };
        }   

        // parses let statements
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