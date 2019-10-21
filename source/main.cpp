#include <iostream>
#include <sstream>
#include <string>
#include <ast_driver.hpp>

// TODO: Capture Ctrl+D and so...
int main(int argc, char **argv)
{
    while (true)
    {
        std::cout << ">> ";

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

        if (root.empty()) continue;

        try
        {
            std::cout << root.evaluate()->toString() << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cout << "error: " << e.what() << std::endl;
        }
    }

    return 0;
}