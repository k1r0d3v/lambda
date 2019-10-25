#include <iostream>
#include <sstream>
#include <string>
#include <ast_driver.hpp>
#include <interpreter.hpp>

bool doYouReallyWantExit()
{
    std::cout << "Do you really want to exit ([y]/n)? ";
    std::cout.flush();

    std::string line;
    std::getline(std::cin, line, '\n');
    return line == "Y" || line == "y" || line.empty();
}

// TODO: Add argv parameters
// TODO: --help, -h
// TODO: --execute <file>, -e <file>
int main(int argc, char **argv)
{
    ast::Context context;
    int lineNumber = 1;
    std::string line;

    for(;;)
    {
        std::cout << TERM_FG_START(TERM_GREEN) << "In ["
                  << TERM_BOLD_START() << TERM_FG_START(TERM_LIGHT_GREEN) << lineNumber << TERM_RESET()
                  << TERM_FG_START(TERM_GREEN) << "]" << TERM_RESET() << ": ";

        // Read until ;
        std::getline(std::cin, line, ';');

        // Check for Ctrl-D
        if (std::cin.eof())
        {
            // Reset std::cin and read trash values
            clearerr(stdin);
            std::cin.clear();

            std::cout << std::endl << std::endl;
            if (doYouReallyWantExit())
                break;
            else
                continue;
        }

        // Do not remove the ; character
        line.append(";");

        // Ignore values after ;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Line ast
        try
        {
            lambda::interpreter::processInterpreterOption(line);
        }
        catch (const lambda::interpreter::ExitRequestException &e)
        {
            if (doYouReallyWantExit()) break;
            else continue;
        }
        catch (const lambda::interpreter::LambdaInterpreterException &e)
        {
            std::cout << e.what() << std::endl << std::endl;
            lineNumber++;
            continue;
        }

        auto root = ast::AST();

        // Parse
        try
        {
            auto stream = std::istringstream(line);
            lambda::ASTDriver::parse(stream, &root, "", false, false);
        }
        catch (const std::exception &e)
        {
            std::cout << TERM_FG_START(TERM_RED) << "ParseError" << TERM_RESET()
                      << ": " << e.what() << std::endl << std::endl;;
            lineNumber++;
            continue;
        }

        // Return if parse fails
        if (root.empty()) continue;

        try
        {
            auto evalResult = root.evaluate(context);
            //If do you want to enable line recording
            //context.setValue("_" + std::to_string(lineNumber), evalResult);

            std::cout << TERM_FG_START(TERM_GREEN) << "Out["
                      << TERM_BOLD_START() << TERM_FG_START(TERM_LIGHT_GREEN)  << lineNumber << TERM_RESET()
                      << TERM_FG_START(TERM_GREEN) << "]"
                      << TERM_RESET() << ": " << evalResult->toString() << std::endl << std::endl;
        }
        catch (const ast::ASTException &e)
        {
            std::cout << TERM_FG_START(TERM_RED)
                      << "---------------------------------------------------------------------------"
                      << TERM_RESET() << std::endl;
            std::cout << e.what() << std::endl << std::endl;
        }

        lineNumber++;
    }

    return 0;
}