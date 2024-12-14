#include <iostream>
#include <string>
#include "translator.h"

void demonstrate_translator_usage()
{
    std::cout << "Enter an arithmetic expression to convert to Reverse Polish Notation: ";

    std::string expression;

    std::getline(std::cin, expression);

    Translator translator(expression);

    std::cout << "Arithmetic expression: " << expression << std::endl;

    try
    {
        translator.tokenizer();
        std::cout << "Expression successfully tokenized." << std::endl;

        translator.parser();
        std::cout << "Expression successfully parsed." << std::endl;

        translator.converter();
        std::cout << "Expression in Reverse Polish Notation: " << translator.get_polish_notation() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main()
{
    demonstrate_translator_usage();
    
    return 0;
}