# Oliver McLaughlin : CSC212 Final Project

## Purpose Overview
My project is a simple programming language which looks and feels similar to something like Lua or Python. It is called Van.

The purpose of this project was to expand my knowledge of programming languages, parsers, and writing large projects. It also gave me the opportunity to really test both myself and the data structures presented within this course.

The name "Van" comes from Willard Van Orman Quine, a logician who is best known for his name being used to denote programs whose output is their own source code.

## Instructions For Use
0. Make sure `g++` and `make` are installed
1. Type `make clean all` into your console where this project is stored.
2. The binary is located in `./bin/`. Run it without any options
for usage
3. Feel free to run and modify any of the programs in `./examples/`. To run a program, type `./bin/van examples/myProgram.van`
4. I have an example program that is meant to be run and read through at the same time, it is located under `./examples/examples.van`
5. This is a fully fledged programming language, if you'd like to
read the documentation, it's located in `./docs/`. The [language guide](docs/language_guide.md) is realtively terse. Otherwhise, look
through and run some of the example programs.
6. Embedding help can be found in the [api document](docs/API.md) also located in `./docs/`

<!-- ## Design Overview
**Design overview: How did you choose to represent the aspects of your program (i.e., what data structures did you choose and why)?**

The most important data structure I used in this project was a tree. The AST (Abstract syntax tree) serves as the only way my program can execute the user input. Although there are other ways to represent programs, some of which have benefits over trees, trees offer a simple and easy to construct way to represent a program.

The AST is comprised of individual nodes which inherit from a base class and all implement the `accept` method, which accepts a visitor which executes the desired funtionality from the node.

For the AST to interact with variables and functions, it has to talk to the context. The context is represented as a class which contains maps which relate strings to data and functions. Speficially, there's a vector of maps (string, `Value`) which associate variable names to their data. Additionally, there is a map which associates strings and pointers to `Callable` objects, which serve the purpose of functions.

All throughout the program I use vectors because they provide the most straightforward solution to variable and unknown user input at almost every conceviable stage of the program. Basically every structure in the language can be recursed, listed, etc. so using vectors helped offload the memory/location logic to a simple dynamic array.

## Code Overview
Like any interpreter, this program takes input from a file and then goes through a set of stages

1. Firstly, the lexer takes the program and splits it into individual lexemes.
2. Next, the parser takes those lexemes and turns them into nodes on an AST.
3. Finally, those nodes are executed and run, thereby running the program.

Variables and functions are stored in the *context*, from which I set and get all dynamic information.

There are four different datatypes in Van (numbers, strings, lists, and nil) (nil cannot be used by the user), but they are all contained under the dynamic type `Value`, which allows for easy casting and interop between types.

The parser constructs the AST nodes but also reports syntactical errors when present. 

For execution I decided to use the [visitor](https://en.wikipedia.org/wiki/Visitor_pattern) data pattern because it is particularly effective on tree data structures and allows me to easily reference the context.

Each AST node inherits from a base class but every node implements the "accept" method, necessary for the visitor to work.

---

Short overview for each file containing code:
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
- There are without a doubt errors and segfaults for things I haven't found yet. This is a relatively large project (In terms of what I've written in the past), and it's difficult to know what to even test. I've tried my best to get most of the basic errors/segfaults out of the way, but there are definitely more I have yet to find.

* I can't promise this interpreter is in any way perfect, I can only promise that I tried my best to fix everything I could and use the data structures learned in this class.

* (Not a bug just unhappy) There's no real way to debug when something is invalid syntax within a Van program, the parser spits out errors but they're pretty unhelpful for the user.

## Easter Eggs
- Using the -j option after your filename, like `./bin/van examples/myfile.van -j`
will print out the AST for your program in JSON. You can copy that JSON into [this website](https://vanya.jp.net/vtree/index.html) to see it as a tree.

- I realized I could have an API for the user to add their own custom builtin functions written in C++, much like how Lua does. So I added a little guide in `./docs/` on how to do this.

## Group
- I did this project alone.

## Topic Overview

- Mastery, to me, is:

    1. Understanding of the topic

    2. Ability to correctly apply the topic &| its functionality

* **Dynamic Array (NN)**

    Parsing provides a situation in which basically everything can have variable length which is only known at parse time. This is a perfect scenario for dynamic arrays.
    - The reason why you would use a dynamic array is if the size
          of your input in unknown at runtime. Parsing user input with as diverse of a range as a programming language is the perfect use case for dynamic arrays because I have no idea about *any length of anything* at compile time.

    A great example would be in `parser.h` line 109, the function `makeProgram`. `makeProgram` collects all the *statements* that comprise the program into a vector which is then passed to a `Block` which serves as the root node of the program. Using a dynamic array here is the cleanest and most logical solution to solving the dynamic and unpredictable nature of the length of a program. By using a dynamic array, I offload all the memory complexity and work to the vector implementation. Using a vector allows me to collect programs with an unknown number of statements. 

* **Sets & Maps (NN)**

    I used maps in several ways in my project. The underlying reason for
    their use is when I need to associate names for things like functions
    and variable names with their associated pointers and values respectively.

    Specifically, in `context.h` on line 46 I represent different levels of scope as a vector of maps which accept a string and return a Value. This works because the back of the vector always contains a map with the current scope, from which I can associate names with variables and resolve unknown identifers easily.

    I used `std::unordered_map` for this project for the quicker average search/deletion/insertion. Additionally, I don't need ordering for variables nor functions, so taking advantage of unordered map's time complexity was useful here.

    There's not really a simpler way of associating user input (identifiers) with complex data structures (pointers to callable objects and values), so using a map here was cruical.

* **Binary Search (NN)**

    `The code for this can be found in builtin.cpp, line 250`
    I have a builtin function in my language called "contains", it returns true
    if a list contains a value. I used binary search here for two main reasons.

    * It improves time complexity O(n) (linear search) to O(log n) for sorted lists

    I slightly modified my base method such that there's compatability for unsorted
    lists as well, but adding binary search here allows for very fast searches on
    sorted lists. And when paired with my quicksort implementation, it increases overall performance when compared to a simple linear search.

* **Recursive Algorithms**

    The biggest way I use recursive algorithms in my project is for the recursive
    descent parser. Although there are more niche ways of using both iteration
    and recursion to do things like expression parsing, an LL(1) recursive
    descent parser is really my only option.

    This approach also makes the most sense: Programs are recursive structures with
    definite patterns of both structure and abstraction. 
 -->
