# Table of Contents

## Purpose Overview

Instructions For Use
1. Type `make` into your console where this project is stored.
2. The binary is located in `./bin/`. Run it without any options
for usage
3. This is a fully fledged programming language, if you'd like to
read the documentation, it's located in `./docs/`. The [language guide](docs/language_guide.md) is realtively terse. Otherwhise, look
through and run some of the example programs.


## Design Overview

## Code Overview

## Potential Bugs
- This is a large program and I wouldn't be surprised if there's some glaring issues I haven't been able to see yet.

As of right now I've identified one memory leak present in AssignmentStatements. This happens because to produce
the IndexExpression necessary to do a self-referential assignment like `+=` I have to allocate memory that shares
the same expression pointers as the indexes, which makes freeing an issue.

* I know for a fact that mismatched quotes, although they don't produce a memory leak,
are UB. Sometimes the resulting error prints mojibake, sometimes it doesn't. It's
a small issue that's purely cosmetic.

## Easter Eggs
- Using the -j option after your filename, like `./bin/van examples/myfile.van -j`
will print out the AST for your program in JSON. You can copy that JSON into [this website](https://vanya.jp.net/vtree/index.html) to see it as a tree.

## Group
- I did this project alone.

## Topic Overview

- Mastery, to me, is:

    A. Understanding of the topic

    B. Ability to correctly apply the topic &| its functionality



* **Dynamic Array (NN)**

    Parsing: Unknown length of basically any number of things
    at runtime provide a perfect place for dynamic arrays.
        - Reason why you would use a dynamic array is if the size
          of your input in unknown at runtime.

* **Maps (NN)**

    I used maps in several ways in my project. The underlying reason for
    their use is when I need to associate names for things like functions
    and variable names with their associated pointers and values respectively.

* **Sorting Algos (Implementation and Analysis) (NN)**

    I implemented a version of Quicksort as a builtin function for my language.
    This is for two main reasons.

    1. Efficiency. This language is interpreted and thus lacks the performance
    of a compiled language. I can aide this along by keeping sorting to C++.

    2. Having some sort of builtin sorting function is pretty necessary to
    get any real work done.

* **Binary Search (NN)**

    I have a builtin function in my language called "contains", it returns true
    if a list contains a value. I used binary search here for two main reasons.

    A) It decreases time complexity O(n) (linear search) to O(log n)

    B) It was the simplest way for me to implement a good sorting algorithm.
        * Due to the nature of my Value class, it would be difficult to
          use C++'s STL search features. Thus, implementing binary search
          allowed me to take advantage of O(log n) search times without
          increasing the complexity of my overall program.

* **DFS (NN)**

    To execute expressions, we first do a DFS to find the terminal element
    on the lefthand side, we then postorder traversal to get the righthand
    element, then execute the operation at the root node. Finding the deepest
    element like this is essential for expression execution because you have
    to evaluate all subtrees of an expression first.

    DFS is built right into how the BinaryExpression class executes. It first
    gets the terminal node on the left, executes upwards, then gets the terminal
    node on the right, executes upwards, and finally applies the binary operation
    to the two values.

* **Recursive Algorithms**
    The biggest way I use recursive algorithms in my project is for the recursive
    descent parser. Although there are more niche ways of using both iteration
    and recursion to do things like expression parsing, an LL(1) recursive
    descent parser is really my only option.

    This approach also makes the most sense: Programs are recursive structures with
    definite patterns of both structure and abstraction. 