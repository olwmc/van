# Language Guide

Van is an imperative programming language with embedding capabilities. It has all of the basic concepts any other language does (variables, loops, conditionals, functions).


Van is similar to Lua and Python, if you have any experience in those, then you'll find programming in Van easy and fun.

### Hello... you!
```lua
local name;
print("Please enter your name: ");

name = input();

println("Hello... " + name + "!");
```

In this example we can see a few things. First, we define a local variable `name` and set it to whatever the user inputs after prompting. 

We then print out `Hello...`, the user's name, and then an exclamation point.

An alternative way of doing this program could look like:

```lua
print("Please enter your name: " );
println("Hello..." + input() + "!");
```

Because executes an entire expression before fulfilling the function call, the `input` call can be placed right in the middle.

### A simple challenge: Fizzbuzz
```lua
defun fizzbuzz(n) as
    -- Loop from i => n
    for(local i = 1; i <=n; i+=1) do
        -- First check if i is a multiple of
        -- 3 and 5
        if(i % 15 == 0) then
            println("Fizzbuzz");

        -- Then check if i is a multiple of 5
        elif(i%5==0) then
            println("Buzz");

        -- Or a multiple of 3
        elif(i%3==0) then
            println("Fizz");

        -- Otherwhise just print i
        else println(i); end
    end
end
```

As you can see, functions in Van are defined with the `defun` keyword.
```bnf
functionDeclaration ::= "defun" "(" expression ")" "as" {statement} "end"
```

Conditions are in the form
```bnf
conditionalStatement ::= "if" "(" expression ")" "then" {statements} 
    ["elif" "(" expression ")" "then" {statements}]
    ["else" {statements}]
    "end"
```

Van also has two loop variations, `for` and `while`
```bnf
forLoop ::= "for" "("
    variableDeclaration ";"
    expression ";"
    assignmentStatement    
")" 

"do" {statement} "end"
```

```bnf
whileLoop ::= "while" "(" expression ")" "do" {statement} "end"
```

### Another one: Bubblesort
```lua
defun bubbleSort(list) as
  local temp;
  local n = len(list);

  for(local i = 0; i < n-2; i += 1) do
    for(local j = 0; j < n-i-1; j += 1) do

      if(list[j] > list[j+1]) then
        temp = list[j];
        list[j] = list[j + 1];
        list[j + 1] = temp;
      end
      
    end
  end

  return list;
end
```

This example is fairly simple, except now we can see indexing expressions and assignments. Additionally, the `len` builtin function is used to take the length of the list

### Making a "split" function
If you're familiar with javascript you're undoubtedly fond of the `split` method. Here's how we would design something similar in Van:

```lua
defun split(str, delim) as
    -- Temporary string and output array
    local tempStr = "";
    local outArr = [];

    -- Loop through the string
    for(local i = 0; i < len(str); i+=1) do

        -- If the index == the delimiter, 
        -- then add it to the array
        if(str[i] == delim) then
            outArr += tempStr;
            tempStr = "";
        
        else 
            -- Otherwhise, just add the index
            tempStr += str[i];
        end
    end

    -- Add the last string
    outArr += tempStr;

    -- Return the array
    return outArr;
end

-- A grocery list:
split("Black beans,Eggs,Milk,Bread,Chicken", ",");
-- This call returns [Black beans, Eggs, Milk, Bread, Chicken]

```

As you can see, we loop through the string looking for the delimiter, if it's found, we append `tempStr` to the output array, otherwhise we add the character to `tempStr`. Afer we've looped through the whole string, we return the array.

## List of builtins
`print`: Prints the input without a newline character

`println`: Prints the input on its own line.

`assert`: Takes a condition and tries to assert it. Exits the program if the assertion is false.

`input`: Prompts the user for input and returns the given string.

`num`: Tries to cast the given input as a number.

`len`: Returns the length of strings and lists.

`type`: Returns the type of a given value. 

`str`: Returns the input as a string

`rand`: Returns a random integer between 0 and RAND_MAX (Compiler dependent).