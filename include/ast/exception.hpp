#ifndef LAMBDA_EXCEPTION_HPP
#define LAMBDA_EXCEPTION_HPP

#include "common.hpp"
#include <terminal_colors.hpp>

namespace ast
{
    class ASTException :  public std::exception
    {
    public:
        ASTException() = default;

        explicit ASTException(const std::string &message)
            : mWhat(message) { }

    public:
        const char *what() const noexcept override
        {
            return mWhat.data();
        }

    protected:
        virtual const char *head() const noexcept {
            return "";
        }

        std::string mWhat;
    };

    class UnexpectedException : public ASTException
    {
    public:
        explicit UnexpectedException(const string &message)
        {
            mWhat = (string)head() + ": " + message;
        }

        const char *head() const noexcept override
        {
            return TERM_FG_START(TERM_RED) "UnexpectedError" TERM_RESET();
        }
    };

    class NameException : public ASTException
    {
    public:
        explicit NameException(const string &message)
        {
            mWhat = (string)head() + ": " + message;
        }

        const char *head() const noexcept override
        {
            return TERM_FG_START(TERM_RED) "NameError" TERM_RESET();
        }
    };

    class TypeException : public ASTException
    {
    public:
        explicit TypeException(const string &message)
        {
            mWhat = (string)head() + ": " + message;
        }

        const char *head() const noexcept override
        {
            return TERM_FG_START(TERM_RED) "TypeError" TERM_RESET();
        }
    };
}

#endif //LAMBDA_EXCEPTION_HPP
