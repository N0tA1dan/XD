#pragma once

#include <vector>
#include <optional>
#include <string>
#include <sstream>
#include <variant>

class Generator{
public:
    Generator(NodeProg prog)
    : m_prog(prog)
    {

    }

    void gen_expression(NodeExpr expression){
        
    }

    void gen_stmt(NodeStmt statement){

        struct StatementVisitor{
            Generator& generator;
            
            void operator()(NodeStmtExit exitstmt){
                    generator.m_output << "    mov rdi, " << exitstmt.expression.var.value.value() << "\n";
                    generator.m_output << "    mov rax, 60\n";
                    generator.m_output << "    syscall\n";
            };

            void operator()(NodeStmtLet stmtlet){
                std::cout << "node exitstmt found";
            };

        };
        // passes *this so it can access m_output
        std::visit(StatementVisitor{*this}, statement.var);
    }

    std::string gen_prog(){


        m_output << "section .text\n";
        m_output <<"global _start\n";
        m_output <<"_start:\n";

        for(auto stmt : m_prog.stmts){
            gen_stmt(stmt);
        }

        return m_output.str();
    }

private:
    NodeProg m_prog;
    std::stringstream m_output;
};