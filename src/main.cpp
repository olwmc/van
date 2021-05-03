/*
Oliver Wendell McLaughlin CSC212 Fall 2021
*/

#include "interpreter.h"

#include <fstream>
#include <iostream>
#include <unordered_map>


typedef std::unordered_map<std::string, bool> Options;

Options parseOptions(int argc, char** argv) {
    Options optionsMap = {
        {"-j",  false},
        {"err", false}
    };

    for(int i = 2; i < argc; i++) {
        // Check if the option is valid
        if(std::string(argv[i]).size() > 2) {
            std::cout << "Invalid option: " << argv[i] << "\n";
            optionsMap["err"] = true;
            break;
        }

        // If it is, then set the options
        else {
            switch(argv[i][1]) {
                case 'j': optionsMap["-j"] = true; break;

                default:
                    std::cout << "Invalid option: " << argv[i] << "\n";
                    optionsMap["err"] = true;
            }
        }
    }

    return optionsMap;
}

int main(int argc, char** argv) {
    std::string fileName = "";

    // Get the filename
    if(argc > 1) {
        fileName = std::string(argv[1]);
    }
    
    // Parse out the options
    Options options = parseOptions(argc, argv);

    // Open the file and read the contents
    std::ifstream infile(fileName);
    std::string file_contents { 
        std::istreambuf_iterator<char>(infile),
        std::istreambuf_iterator<char>()
    };

    // If there's no file, just print out the usage
    if(argc == 1) {
        std::cout << "The Van Programming Language interpreter\n";
        std::cout << "\tUsage: filename [-j|-h]\n";
        std::cout << "\tRepo link: https://github.com/olwmc/Van\n";

        return 0;
    }

    // If there's an options error print error
    if(options["err"]) {
        std::cout << "Please provide correct options if any\n";
        return 1;
    }
    
    // If the file is good, make an interpreter and check if JSON
    if(infile.good()) {
        Van_Interpreter interpreter(file_contents);

        if(options["-j"]) {
            interpreter.showJson();
        } else {
            interpreter.run();
        }

        return 0;
    }

    // Otherwhise if the file is bad tell the user
    else {
        std::cout << "Please provide a file to execute. Run with no options for help.\n";
        return 1;
    }
}