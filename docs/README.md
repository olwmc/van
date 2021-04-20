# Table of Contents

Purpose Overview

[Instructions For Use](language_guide.md)

Design Overview

Code Overview

Potential Bugs

Easter Eggs

Group

Mastery, to me, is:
    A. Understanding of the topic
    B. Ability to correctly apply the topic/its functionality

Topic Overview
* Dynamic Array
    How do I even show this I have like a million being used
    in varying ways

* Hashtable/Map <= A) Variable scope B) Functions

* Recursive Algorithms <= Recursive Descent parser / Visitor

* Intro to Trees (Within the umbrella topic, I think lab 5)
    Recursive nature of AST, Traversal aspect of AST's, etc.

* Stacks
    I use a std::vector much in the same way that you would use a stack,
    but in this case it's just indexable. I find this particularly useful
    because it gives me all the fifo proporties of a stack, with the
    indexabiliy and searchability of a dynamic array.


* Heap (Expression evaluation)
    Although not traditional, an expression tree is a heap,
    but instead of ordering based on a key, it orders based
    on precedence. When using an expression tree, we actually
    do a depth first search to find the element with the highest
    precedence, which is then evaluated and returned.

    It's self balancing in that the recursive nature of the parser
    always keeps the expression with the lowest precedence higher
    than those with higher precedence.

    I.e. 2*3 =>
        *
       / \
      2   3

    1+2*3 =>

            +
           / \
          1   *
             / \
            2   3

    This is an example of the self balancing based on precedence.
    \* has a higher precedence than +, so the + node stays on top.

    In its entirety, evaluation of an expression tree is O(n)
    because you must visit every node, but by nature of the 
    precedence ordering, the complexity of finding the node to
    execute first is O(n) in the worst case and θ(log n) on
    average