#include <iostream>
#include <variant>
#include <vector>

struct A{
    int x;
};

struct B {
    float x;
};


int main(){
    std::variant<B, A> val;
    B b = {5.5};
    val = b;

    struct visitor{
        void operator()(B& b){
            std::cout << "struct b was found";
        }

        void operator()(A& a){
            std::cout << "struct a was found";
        }
    };
    std::visit(visitor{}, val);
}