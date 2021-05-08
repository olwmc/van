/*
Oliver Wendell McLaughlin CSC212 Spring 2021
*/

#include "forward.h"
#include "value.h"
#include "visitor.h"
#include "context.h"
#include "builtin.h"

#include <iostream>
#include <stdlib.h>
#include <ctime>

/* User defined function */
Value ProgramVisitor::visit(UserFunction &userFunction)
{
    Value v = userFunction.body()->accept(*this);
    return v;
}

Value ProgramVisitor::visit(Builtin &builtin)
{
    Value v = builtin.execute(*this->m_context);
    return v;
}

/* Builtin println function, takes and input and prints it on its own line */
Value builtin_Println::execute(Context &context)
{
    // Get the input from context
    // Using "printStatement.args()" instead of __input__ to make this more
    // maintainable
    Value input = context.resolveVariable(this->args()[0]);

    // Print the input
    std::cout << input.toString() << "\n";

    // Return NIL
    return Value();
}

/* Asserts a given condition */
Value builtin_Assert::execute(Context &context)
{
    // Assert that the value is true
    Value condition = context.resolveVariable(this->args()[0]);

    if (condition.type() == Value_Type::NUMBER && condition.asNumber() == 1)
    {
        return Value("ASSERTION PASSED");
    }

    else
    {
        throw std::runtime_error("FAILED TO ASSERT");
    }
}

/* Takes in input and returns it as a string */
Value builtin_Input::execute(Context &context)
{
    (void)context;

    std::string input;
    std::getline(std::cin, input);

    return Value(input);
}

/* Prints input without newline */
Value builtin_Print::execute(Context &context)
{
    std::cout << context.resolveVariable(this->m_args[0]).toString();
    return Value();
}

/* Casts to number if possible */
Value builtin_NumCast::execute(Context &context)
{
    std::string str = context.resolveVariable(this->m_args[0]).toString();

    double num;

    try
    {
        num = std::stod(str);
    }
    catch (...)
    {
        throw std::runtime_error("Invalid input for number cast");
    }

    return Value(num);
}

/* Length of a list/string */
Value builtin_Len::execute(Context &context)
{
    // Check if arr or str
    Value val = context.resolveVariable(this->m_args[0]);

    if (val.type() == Value_Type::LIST)
    {
        return val.asList().size();
    }

    else if (val.type() == Value_Type::STRING)
    {
        return val.asString().size();
    }
    else
    {
        throw std::runtime_error("Cannot get length of: " + val.toString());
    }
}

Value builtin_Type::execute(Context &context)
{
    Value val = context.resolveVariable(this->m_args[0]);

    std::string type;

    switch (val.type())
    {
    // number string list nil
    case NUMBER:
        type = "number";
        break;
    case STRING:
        type = "string";
        break;
    case LIST:
        type = "list";
        break;

    default:
        type = "nil";
    }

    return Value(type);
}

Value builtin_StrCast::execute(Context &context)
{
    Value val = context.resolveVariable(this->m_args[0]);

    return Value(val.toString());
}

Value builtin_Rand::execute(Context &context)
{
    (void)context;
    return Value(rand());
}

Value builtin_Sort::execute(Context &context)
{
    Value val = context.resolveVariable(this->m_args[0]);
    int size = val.asList().size();

    Value arr[size];

    for (int i = 0; i < size; i++) {
        arr[i] = val.asList()[i];
    }

    quickSort(arr, 0, size - 1);

    std::vector<Value> ret;

    for (int i = 0; i < size; i++) {
        ret.push_back(arr[i]);
    }

    return Value(ret);
}

int builtin_Sort::partition(Value arr[], int low, int high)
{
    int pivdex = (low + high) / 2;
    std::swap(arr[pivdex], arr[high]);

    int i = low;

    Value check;
    for (int j = low; j < high; j++)
    {   
        // First check if the comparison returns an error
        check = (arr[j] < arr[high]);

        // If it does, throw an error
        if(check.type() == Value_Type::ERR) {
            throw std::runtime_error("Sorting requires uniform types");
        }

        // Else check if it's true, if so swap and increment i
        if (check.asNumber())
        {
            std::swap(arr[i], arr[j]);
            i++;
        }
    }

    // Swap
    std::swap(arr[i], arr[high]);

    //return index of pivot
    return i;
}

void builtin_Sort::quickSort(Value arr[], int low, int high)
{
    //base case, takes care of the same case we hinted on in mergesort
    if (low >= high) { return; }
    int pivotIndex = partition(arr, low, high);

    quickSort(arr, low, pivotIndex - 1);
    quickSort(arr, pivotIndex + 1, high);
}

Value builtin_Contains::execute(Context &context)
{
    Value list = context.resolveVariable(this->m_args[0]);
    Value val = context.resolveVariable(this->m_args[1]);
    // Value sorted = builtin_Sort().execute(context);

    int res = binarySearch(list.asList(), 0, list.asList().size(), val);

    if(res != -1) { return res; 
    }
    // If it's not found, try a linear search
    for(int i = 0; i < (int)list.asList().size(); i++) {
        if( (list.asList()[i] == val).asNumber() ) {
            return i;
        }
    }

    return -1;
}

int builtin_Contains::binarySearch(std::vector<Value> arr, int low, int high, Value v) {
    if (high < low) { return -1; }

    int mid = (low + high) / 2;
    
    // Check if mid == v
    if ( (arr[mid] == v).asNumber() ) {
        return mid;
    }

    // Check if mid > v
    else if ( (arr[mid] > v).asNumber() ) {
        return binarySearch(arr, low, mid - 1, v);
    }

    // Otherwhise mid < v
    return binarySearch(arr, mid + 1, high, v);
 
    return -1;
}