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
                generator.m_output << "    " << int_lit.int_lit.value.value() << "\n";
            }

            // always moves the variables value into rax
            void operator()(NodeIdent identifier){

                std::unordered_map<std::string,size_t>::const_iterator got = generator.m_variables.find (identifier.ident.value.value());

                if ( got == generator.m_variables.end() ){
                    std::cerr << "Error: variable not declared";
                    exit(EXIT_FAILURE);
                }else {
                    generator.m_output;
                    generator.m_output << "    mov rax, [rbp - " << generator.m_variables[identifier.ident.value.value()] * 4 <<"]\n";
                }
            }
            
        };

        std::visit(ExprVisitor{*this}, expression.var);
    }

    void gen_stmt(NodeStmt statement){

        struct StatementVisitor{
            Generator& generator;
            
            void operator()(NodeStmtExit exitstmt){
                    generator.gen_expression(exitstmt.expression); 
                    generator.m_output << "    mov rdi, rax\n";
                    generator.m_output << "    mov rax, 60\n";
                    generator.m_output << "    syscall\n";
            };

            // the expression is actually generated here. essentially it was either using std::holds_alternative or making a different gen_expr function explicitlly for let statements
            void operator()(NodeStmtLet stmtlet){

                generator.m_variables.insert({stmtlet.identifier.value.value(), generator.m_stack_size});

                // checks if the variable is set to an immediate value like let x = 10;
                if(std::holds_alternative<NodeIntLit>(stmtlet.expression.var)){
                    generator.m_output << "    mov DWORD [rbp - " << generator.m_stack_size * 4 << "], " << std::get<NodeIntLit>(stmtlet.expression.var).int_lit.value.value() << "\n";
                }
                // checks if the variable is assigned to another variable such as let y = x;
                // since we cant move from memory to memory, we have to store the value we are accesing into a temporary register like rax in this instance
                if(std::holds_alternative<NodeIdent>(stmtlet.expression.var)){
                    NodeIdent identifier = std::get<NodeIdent>(stmtlet.expression.var);
                    std::unordered_map<std::string,size_t>::const_iterator got = generator.m_variables.find (identifier.ident.value.value());

                    if ( got == generator.m_variables.end() ){
                        std::cerr << "Error: variable not declared";
                        exit(EXIT_FAILURE);
                    }else {
                        generator.m_output;
                        generator.m_output << "    mov rax, [rbp - " << generator.m_variables[identifier.ident.value.value()] * 4 <<"]\n";
                        generator.m_output << "    mov [rbp - " << generator.m_stack_size * 4 << "], rax\n";
                    }
                }
                // increment stack size because we added a variable
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
        // stack frame created here.
        // i would like to do this for every function created then id have to make a new scope.
        m_output << "    push rbp\n";
        m_output << "    mov rbp, rsp\n";

        for(auto stmt : m_prog.stmts){
            gen_stmt(stmt);
        }
        // end of stack frame. restores rbp
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