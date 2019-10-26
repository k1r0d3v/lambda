#ifndef LAMBDA_INTERPRETER_HPP
#define LAMBDA_INTERPRETER_HPP

#include <exception>
#include <regex>
#include <fstream>
#include <streambuf>
#include <string>
#include <terminal_colors.hpp>


namespace lambda::interpreter
{

    class LambdaInterpreterException : public std::exception
    {
    protected:
        virtual const char *head() const noexcept = 0;
    };

    class ExitRequestException : public LambdaInterpreterException
    {
    public:
        const char *head() const noexcept override
        {
            return TERM_FG_START(TERM_RED) "ExitRequestError" TERM_RESET();
        }

        const char *what() const noexcept override
        {
            return head();
        }
    };

    class FileNotFoundException : public LambdaInterpreterException
    {
    public:
        explicit FileNotFoundException(const std::string &filename)
        {
            mWhat = (std::string)head() + ": File \'" + filename + "\' can not be found";
        }

        const char *head() const noexcept override
        {
            return TERM_FG_START(TERM_RED) "FileNotFoundError" TERM_RESET();
        }

        const char *what() const noexcept override
        {
            return mWhat.data();
        }

    private:
        std::string mWhat;
    };


    class UnknownOptionException : public LambdaInterpreterException
    {
    public:
        explicit UnknownOptionException(const std::string &value)
        {
            mWhat = (std::string)head() + ": The option \'" + value + "\' do not exists";
        }

        const char *head() const noexcept override
        {
            return TERM_FG_START(TERM_RED) "UnknownOptionError" TERM_RESET();
        }

        const char *what() const noexcept override
        {
            return mWhat.data();
        }

    private:
        std::string mWhat;
    };

    static void processInterpreterOption(std::string &line)
    {
        std::regex r("^[ \n\t]*#(.*?)[ \n\t]*;");
        std::smatch m;

        if (std::regex_search(line, m, r))
        {
            line.clear(); // Clear the line, we have an option
            auto value = m[1].str();

            if (value == "exit")
            {
                throw ExitRequestException();
            }
            else if (value.rfind("open ", 0) == 0) // starts with #open
            {
                auto filename = value.substr(5);
                std::ifstream file(filename);

                if (file.is_open())
                {
                    // Read the file
                    line = std::string((std::istreambuf_iterator<char>(file)),
                                       std::istreambuf_iterator<char>());
                    // And close
                    file.close();
                }
                else throw FileNotFoundException(filename);
            }
            else throw UnknownOptionException(value);
        }
    }
}

#endif //LAMBDA_INTERPRETER_HPP
