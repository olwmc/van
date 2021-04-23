#include "interpreter.h"

#include <fstream>
#include <iostream>
#include <unordered_map>


typedef std::unordered_map<std::string, bool> Options;

Options parseOptions(int argc, char** argv) {
    Options optionsMap = {
        {"-h",  false},
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
                case 'h': optionsMap["-h"] = true; break;
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

    if(argc > 1) {
        fileName = std::string(argv[1]);
    }
    
    Options options = parseOptions(argc, argv);

    std::ifstream infile(fileName);
    std::string file_contents { 
        std::istreambuf_iterator<char>(infile),
        std::istreambuf_iterator<char>()
    };

    if(options["err"]) {
        std::cout << "Please provide correct options if any\n";
    }

    else if(options["-h"]) {
        std::cout << "The Van Programming Language interpreter\n";
        std::cout << "\tUsage: filename [-j|-h]\n";
        std::cout << "\tRepo link: https://github.com/olwmc/Van\n";
    }
    
    else if(infile.good()) {
        Van_Interpreter interpreter(file_contents);

        if(options["-j"]) {
            interpreter.showJson();
        } else {
            interpreter.run();
        }
    }
    
    else {
        std::cout << "Please provide a valid file to run. Run with -h for help\n";
    }
}