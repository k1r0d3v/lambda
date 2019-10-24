#include <iostream>
#include <sstream>
#include <string>
#include <ast_driver.hpp>
#include "interpreter.hpp"


// TODO: Capture Ctrl+D and so...
// TODO: Add argv parameters
// TODO: --help, -h
// TODO: --execute <file>, -e <file>
int main(int argc, char **argv)
{
    int lineNumber = 1;

    while (true)
    {
        std::cout << "In [" << lineNumber << "]: ";

        std::string line, tmp;
        std::getline(std::cin, line, ';');
        std::getline(std::cin, tmp, '\n'); // Remove the trailing string after the ;
        line.append(";"); // Do not remove the new line character

        try
        {
            lambda::interpreter::processInterpreterOption(line);
        }
        catch (lambda::interpreter::ExitRequestException &e)
        {
            std::cout << "Do you really want to exit ([y]/n)? ";
            std::getline(std::cin, line, '\n');
            if (line == "Y" || line == "y" || line.empty())
                break;
            else continue;
        }
        catch (lambda::interpreter::LambdaInterpreterException &e)
        {
            std::cout << e.name() << ": " << e.what() << std::endl;
            continue;
        }

        // Parse
        auto stream = std::istringstream(line);
        auto root = ast::AST();

        try
        {
            lambda::ASTDriver::parse(stream, &root, "", false, false);
        }
        catch (const std::exception &e)
        {
            std::cout << "ParseException: " << e.what() << std::endl;
        }

        // Return if parse fails
        if (root.empty()) continue;

        try
        {
            //std::cout << root.toString() << std::endl;
            std::cout << "Out[" << lineNumber << "]: " << root.evaluate()->toString() << std::endl << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cout << "EvaluationException: " << e.what() << std::endl;
        }

        lineNumber++;
    }

    return 0;
}