// ast.hpp allows to print the ast tree and give a visual representation


#pragma once

#include <iostream>
#include <sstream>
#include <variant>


class AST{

public:
    AST(NodeProg prog) 
    : m_prog(prog)
    {

    }

    void print_expression(NodeExpr expression){
        pretty_ast << "    Expression:\n";
        pretty_ast << "      Value:";
        pretty_ast << " " << expression.var.value.value()<< "\n";  
    }

    void print_stmt(NodeStmt statement){
        struct StatementVisitor{
            AST&  ast;

            void operator()(NodeStmtExit exit_stmt){
                ast.pretty_ast << "  NodeStmtExit:\n";
                ast.print_expression(exit_stmt.expression);
            }

            void operator()(NodeStmtLet stmt_let){
                ast.pretty_ast << "  NodeStmtLet:\n";
                ast.pretty_ast << "    Identifier: " << stmt_let.identifier.value.value() << "\n"; 
                ast.print_expression(stmt_let.expression);
            }
        };
        std::visit(StatementVisitor{*this}, statement.var);

    }

    void print_ast(){
        std::cout << "Prog:\n";
        for(auto statement : m_prog.stmts){
            print_stmt(statement);
        }

        std::cout << pretty_ast.str();
    }

private:
    NodeProg m_prog;
    // a visual representation of the ast tree
    std::stringstream pretty_ast;
};