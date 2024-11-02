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
