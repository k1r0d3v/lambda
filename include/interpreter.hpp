#ifndef LAMBDA_INTERPRETER_HPP
#define LAMBDA_INTERPRETER_HPP

#include <exception>
#include <regex>
#include <fstream>
#include <streambuf>
#include <string>


namespace lambda::interpreter
{

    class LambdaInterpreterException : public std::exception
    {
    public:
        virtual const char *name() const = 0;
    };

    class ExitRequestException : public LambdaInterpreterException
    {
    public:
        const char *name() const override
        {
            return "ExitRequestException";
        }

        const char *what() const noexcept override
        {
            return "";
        }
    };

    class FileNotFoundException : public LambdaInterpreterException
    {
    public:
        explicit FileNotFoundException(const std::string &filename)
        {
            mWhat = (std::string) "File \'" + filename + "\' can not be found";
        }

        const char *name() const override
        {
            return "FileNotFound";
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
            mWhat = (std::string) "The option \'" + value + "\' do not exists";
        }

        const char *name() const override
        {
            return "UnknownOption";
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
