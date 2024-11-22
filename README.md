# XD Compiler
XD compiler is a very dumb compiler that im making as a small project meant for a C type language.

to compile:
    `cd build`
    `cmake ..`
    `make`

november 2nd 2024:
    basically i was making this compiler in october in github codespaces, but it got deleted. So here i am because of githubs idiotic system. Anyways I have to write the parser again.

    I was writing earlier about having issues with writing grammar and left recursion. Left recursion is basically when a productions left most non-terminal is itself. 

    For example:

    S ::= S"x"

    as you can see here, S is a non-terminal and "x" is any string or terminal in this particular case. Since im making a recursive descent parser which kinda acts like a ll(1) parser, we dont want left recursion. We would fall into an infinite loop checking if S exists, since it does, we keep going and we will have no clue when to terminate. 

    A solution is to put a terminal before the non-terminal like so:

    S ::= "x"S

    So here we could implement in our code to "derive forward" until we dont see the string/token.

    here im using string as an example, but it could be another non-terminal or token thats not the actual non-terminal of the production.


    remember a recursive descent parser uses recursive (or non recursive) functions that match the grammar directly.


November 3rd 2024:
    changed grammar and changed parser.


November 6 2024:
    im trying to understand how std::visit and std::variant work.

    heres how im implemented it

    https://stackoverflow.com/questions/66961406/c-variant-visit-overloaded-function
    https://en.cppreference.com/w/cpp/utility/variant/visit
    https://levelup.gitconnected.com/understanding-std-visit-in-c-a-type-safe-way-to-traverse-variant-objects-dbeff9b47003

    from my understanding, std::visit basically determines the type of a std::variant then does some sort of function overloading then selects the appropriate function at runtime. i think this has something to do with dispatching as well

    answer i found from gpt:

    Internally, std::visit relies on runtime type information (RTTI) or a form of type indexing to determine the type of the object inside the variant.

    A std::variant is essentially a type-safe union, meaning it can hold one of several types at any given moment.

    std::variant internally stores an index (a std::size_t or similar) that indicates which of the alternative types in the variant is currently being held.

    Each type in the variant has an associated index, which is fixed at compile time. This index is essentially the position of the type in the list of types Ts... provided to std::variant<Ts...>.
    
    The type index allows std::visit to know the currently active type, so it can correctly call the appropriate visitor function.

Novemeber 7 2024:
    Added printing for the parse tree. you can now use print_prog() to print the entire programs ast tree.


November 18 2024:
    Going to try to implement a stack frame for local variables.

    what i learned today.

    rsp points to the most recently pushed element onto the stack.

    for example

    global _start


    _start:
        push 100
        push 200

        mov rax, [rsp] ;; contains 200

        NOP

    rax register now contains 200.


    global _start


    _start:
        push 100
        push 200

        mov rax, [rsp+8] ;; contains 100

        NOP

    [rsp+8] just means 8 bytes ahead, since this is x64, registers are 64 bits in lenght, aka 8 bytes, so this would get the value 100 and move it into rax


November 19 2024:

    RSP points to the top of the stack. 

    RBP points to the "base" of the stack frame

    for example

    global _start

_start:
    push rbp
    mov rbp, rsp

    push 10
    push 20
    
    mov rax, [rbp - 8]

    pop rbp


    NOP

    here its 'rbp - 8' because rbp points to the base of the stack frame. 'rbp - 0' is just the return address i believe or the address of the previous rbp value.

    intution: for each new function, make a new stack frame. this can be achieved by whenever we occur a fucntion decleration, we can do the following:
    
    foo:
        push rbp
        mov rbp, rsp

    This is how all functions are made in the gcc compiler even if they dont contain any variables or return anything.

november 21 2024:
    I added a stack frame system almost. it doesnt work for scopes yet so maybe i have to make a vector with different scopes.

    What i did was added all declared variables into a hash map, the key was the identifier and the value was the stack position.

    also for now, every int is considered to be 32-bit.

    one issue i have now is assigning variables to other variables.

    for example:

    let x = 10;
    let y = x;

    currently this is what my compiler generates:
    mov DWORD [rbp - 4],   10
    mov DWORD [rbp - 8],  [rbp - 4]

    i dont believe memory to memory is permitted, so we need to move the value x into rax, then move rax into y, like so:

    mov DWORD [rbp - 4],   10
    mov rax, [rbp - 4]
    mov DWORD [rbp - 8],  rax

    i dont know where to start and may need some corrections on the entire generation. should be relatively simple though.

    this thread seemed interesting: https://news.ycombinator.com/item?id=15249994

    basically instead of using std::visit, they use the index member of std::variant.
    They use a switch statement on the index to determine the types. I might actually implement this.