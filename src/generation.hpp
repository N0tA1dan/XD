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
        struct ExprVisitor{
            Generator& generator;

            void operator()(NodeIntLit int_lit){
                generator.m_output << "  " << int_lit.int_lit.value.value() << "\n";
            }

            void operator()(NodeIdent identifier){
                /* 

                TODO: 
                check if variable has been declared already and actually exists. 
                then return/move it from the position it is in the stack
                
            
                */
            }
        };

        std::visit(ExprVisitor{*this}, expression.var);
    }

    void gen_stmt(NodeStmt statement){

        struct StatementVisitor{
            Generator& generator;
            
            void operator()(NodeStmtExit exitstmt){
                    generator.m_output << "    mov rdi,";
                    generator.gen_expression(exitstmt.expression);
                    generator.m_output << "    mov rax, 60\n";
                    generator.m_output << "    syscall\n";
            };

            void operator()(NodeStmtLet stmtlet){
                /*
                TODO:
                Add the variable identifier and value to the m_vars. 
                */
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

    void push(const std::string &reg){
        m_output << "    push " << reg << "\n";
        m_stack_size++;
    }

    void pop(const std::string &reg){
        m_output << "    pop " << reg << "\n";
        m_stack_size--;
    }

    struct variable{
        std::string identifier;
        size_t stack_location;
    };

    NodeProg m_prog;
    std::stringstream m_output;
    std::vector<variable> m_variables;
    size_t m_stack_size = 0;
};