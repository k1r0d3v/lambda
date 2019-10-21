#include <iostream>
#include <sstream>
#include <string>
#include <ast_driver.hpp>

// TODO: Capture Ctrl+D and so...
int main(int argc, char **argv)
{
    int lineNumber = 1;

    while (true)
    {
        std::cout << "In [" << lineNumber << "]: ";

        std::string line;
        std::getline(std::cin, line);

        if (line.empty()) continue;
        if (line == "exit") break;

        // Do not remove the new line character
        line.append("\n");

        // Parse
        auto stream = std::istringstream(line);
        auto root = ast::AST();

        try
        {
            lambda::ASTDriver::parse(stream, &root, "", false, false);
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }

        // Return if parse fails
        if (root.empty()) continue;

        try
        {
            std::cout << "Out[" << lineNumber << "]: " << root.evaluate()->toString() << std::endl << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cout << "error: " << e.what() << std::endl;
        }

        lineNumber++;
    }

    return 0;
}