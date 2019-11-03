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

void getline(std::istream &is, std::string &line, const std::string& delim)
{
    line.clear();

    while(true)
    {
        std::string tmp;
        std::getline(std::cin, tmp);
        line += tmp;

        auto index = line.find(delim);
        if (index != std::string::npos)
        {
            line = line.substr(0, index + delim.length());
            return;
        }
    }
}

// TODO: Add argv parameters
// TODO: --help, -h
// TODO: --execute <file>, -e <file>
int main(int argc, char **argv)
{
    ast::TypeContext typeContext;
    ast::Context context;
    int lineNumber = 1;
    std::string line;

    for(;;)
    {
        std::cout << TERM_FG_START(TERM_GREEN) << "In ["
                  << TERM_BOLD_START() << TERM_FG_START(TERM_LIGHT_GREEN) << lineNumber << TERM_RESET()
                  << TERM_FG_START(TERM_GREEN) << "]" << TERM_RESET() << ": ";

        // Read until ;
        getline(std::cin, line, ";;");

        // Check for Ctrl-D
        if (std::cin.eof())
        {
            // Reset std::cin and read trash values
            //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearerr(stdin);
            std::cin.clear();

            std::cout << std::endl << std::endl;
            if (doYouReallyWantExit())
                break;
            else
                continue;
        }

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
            auto typeResult = root.typecheck(typeContext);
            auto evalResult = root.evaluate(context);

            // Line store
            auto lineIdName = (ast::string)"_" + ast::to_string(lineNumber);
            typeContext.setTypeFor(lineIdName, typeResult);
            context.setValue(lineIdName, evalResult);
            //

            std::cout << TERM_FG_START(TERM_GREEN) << "Out["
                      << TERM_BOLD_START() << TERM_FG_START(TERM_LIGHT_GREEN)  << lineNumber << TERM_RESET()
                      << TERM_FG_START(TERM_GREEN) << "]"
                      << TERM_RESET() << ": "
                      << evalResult->toString() << " ;; " << typeResult->toString() << std::endl << std::endl;
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