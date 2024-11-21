#pragma once

#include <vector>
#include <optional>
#include <string>
#include <sstream>
#include <variant>
#include <unordered_map>

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
                
                /*
                This almost works. the only issue is when i attempt to assign variables to other declared variables
                this can be solved by moving the value from the stack into a register like rax, then moving rax into the targetted stack position
                */

                std::unordered_map<std::string,size_t>::const_iterator got = generator.m_variables.find (identifier.ident.value.value());

                if ( got == generator.m_variables.end() ){
                    std::cerr << "Error: variable not declared";
                    exit(EXIT_FAILURE);
                }else {
                    generator.m_output
                    generator.m_output << "    [rbp - " << generator.m_variables[identifier.ident.value.value()] * 4 <<"]\n";
                }
            }
            
        };

        std::visit(ExprVisitor{*this}, expression.var);
    }

    void gen_stmt(NodeStmt statement){

        struct StatementVisitor{
            Generator& generator;
            
            void operator()(NodeStmtExit exitstmt){
                    // have to change this to mov rax into rdi. 
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

                generator.m_variables.insert({stmtlet.identifier.value.value(), generator.m_stack_size});
                generator.m_output << "    mov DWORD [rbp - " << (generator.m_stack_size  * 4 ) << "], "; 
                generator.gen_expression(stmtlet.expression);

                generator.m_stack_size++;
            };

        };
        // passes *this so it can access m_output
        std::visit(StatementVisitor{*this}, statement.var);
    }

    std::string gen_prog(){


        m_output << "section .text\n";
        m_output <<"global main\n";
        m_output <<"main:\n";
        m_output << "    push rbp\n";
        m_output << "    mov rbp, rsp\n";

        for(auto stmt : m_prog.stmts){
            gen_stmt(stmt);
        }

        m_output << "    LEAVE";

        return m_output.str();
    }

private:

    struct variable{
        std::string identifier;
        size_t stack_location;
    };

    NodeProg m_prog;
    std::stringstream m_output;
    // essentially keeps track of known variables and their position in the stack. this should have a O(1) time complexity for find(). pretty fast lookup
    std::unordered_map<std::string, size_t> m_variables;
    // rbp-0 is always just the original rbp value. this still holds a value which is not the pointer we currently want to access, so we account for the 1 value it currently holds.
    size_t m_stack_size = 1;
};