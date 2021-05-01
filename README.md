# Oliver McLaughlin : CSC212 Final Project

## Purpose Overview
My project is a simple programming language which looks and feels similar to something like Lua or Python. It is called Van.

The purpose of this project was to expand my knowledge of programming languages, parsers, and writing large projects. It also gave me the opportunity to really test both myself and the data structures presented within this course.

The name "Van" comes from Willard Van Orman Quine, a logician who is best known for his name being used to denote programs whose output is its own source code.

## Instructions For Use
1. Type `make` into your console where this project is stored.
2. The binary is located in `./bin/`. Run it without any options
for usage
3. Feel free to run and modify any of the programs in `./examples/`. To run a program, type `./bin/van examples/myProgram.van`
4. This is a fully fledged programming language, if you'd like to
read the documentation, it's located in `./docs/`. The [language guide](docs/language_guide.md) is realtively terse. Otherwhise, look
through and run some of the example programs.


## Design Overview


## Code Overview
Much like any interpreter, this program takes input from a file and then goes through a set of stages

1. Firstly, the lexer takes the program and splits it into individual lexemes.
2. Next, the parser takes those lexemes and turns them into nodes on an AST.
3. Finally, those nodes are executed and run, thereby running the program.

Variables and functions are stored in the *context*, from which I set and get all dynamic information.

There are three different datatypes in Van (numbers, strings, lists, and nil), but they are all contained under the dynamic type `value`, which allows for easy casting and interop between types.

The parser constructs the AST nodes but also reports syntactical errors when present. 

For execution I decided to use the [visitor](https://en.wikipedia.org/wiki/Visitor_pattern) data pattern because it is particularly effective on tree data structures and allows me to easily reference the context.

Each AST node inherits from a base class but every node implements the "accept" method, necessary for the visitor to work.


* *AST.h*
    - Contains all the classes for the AST nodes as well as their associated information and JSON representation.

* *builtin.h*
    - Contains all the classes for the builtin functions like print and length

* *callable.h*
    - Provides the base code for `callable` objects which relate to both builtin and user defined functions

* *context.h*
    - Contains the class info and methods for the context from which functions and variables are resolved
    and changed

* *forward.h*
    - Provides forward declarations so that the visitor data pattern works cleanly

* *interpreter.h*
    - Contains the class and basic infrastructure for an interpreter, including access to the context

* *lexer.h*
    - Class, methods, and other data relating to the lexing 
    
* *parser.h*
    - Contains the entirety of the parser which both parses the lexemes into an AST as well as reports syntactical errors

* *value.h*
    - Contains the class and methods for the dynamically typed `value` type. `value` is the only datatype in van and thus requires
    logic to override typical operators

* *visitor.h*
    - Provides the class and method backing for the visitor which executes the program

---

* *AST.cpp*
    - Contains all of the visit methods for all the AST nodes, effectively carrying out the logic of each individual node.

* *builtin.cpp*
    - Contains the logic for each of the builtin function classes

* *context.cpp*
    - Contains all the methods for setting and getting variables (and their indexes), as well as functions.

* *lexer.cpp*
    - Implements all the methods as described in lexer.h; Carries out the lexing stage

* *main.cpp*
    - Spawns an interpreter and parses out command line options/file input

* *value.cpp*
    - Implements and handles all the necessary methods for the `value` type.

## Potential Bugs
- This is a large program and I wouldn't be surprised if there's some glaring issues I haven't been able to see yet.

- There's some hacky fixes for memory leaks (most notably in assignment statements which lack a compound expression). 

- There are without a doubt errors and segfaults for things I haven't found yet. This is a relatively large project (In terms of what I've written in the past), and it's difficult to know what to even test. I've tried my best to get most of the basic errors/segfaults out of the way, but there are definitely more I have yet to find.

* I know for a fact that mismatched quotes, although they don't produce a memory leak,
are UB. Sometimes the resulting error prints mojibake, sometimes it doesn't. It's
a small issue that's purely cosmetic.

## Easter Eggs
- Using the -j option after your filename, like `./bin/van examples/myfile.van -j`
will print out the AST for your program in JSON. You can copy that JSON into [this website](https://vanya.jp.net/vtree/index.html) to see it as a tree.

- I realized I could have an API for the user to add their own custom builtin functions written in C++, much like how Lua does. So I added a little guide in `./docs/` on how to do this.

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

* **Binary Search (NN)**

    I have a builtin function in my language called "contains", it returns true
    if a list contains a value. I used binary search here for two main reasons.

    A) It decreases time complexity O(n) (linear search) to O(log n)

    B) It was the simplest way for me to implement a good sorting algorithm.
        * Due to the nature of my Value class, it would be difficult to
          use C++'s STL search features. Thus, implementing binary search
          allowed me to take advantage of O(log n) search times without
          increasing the complexity of my overall program.

* **Sorting Algos (Implementation and Analysis) (NN)**

    I implemented a version of Quicksort as a builtin function for my language.
    This is for two main reasons.

    1. Efficiency. This language is interpreted and thus lacks the performance
    of a compiled language. I can aide this along by keeping sorting to C++.

    2. Having some sort of builtin sorting function is pretty necessary to
    get any real work done.

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