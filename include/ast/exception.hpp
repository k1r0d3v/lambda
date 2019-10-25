#ifndef LAMBDA_EXCEPTION_HPP
#define LAMBDA_EXCEPTION_HPP

#include "common.hpp"
#include "terminal_colors.hpp"

namespace ast
{
    // TODO: ASTException, EvaluationException, ...

    class ASTException :  public std::exception
    {
    protected:
        virtual const char *head() const noexcept = 0;
    };

    class EvaluationException : public ASTException { };

    class NameException : public EvaluationException
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

        const char *what() const noexcept override
        {
            return mWhat.data();
        }

    private:
        string mWhat;
    };

    class TypeException : public EvaluationException
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

        const char *what() const noexcept override
        {
            return mWhat.data();
        }

    private:
        string mWhat;
    };
}

#endif //LAMBDA_EXCEPTION_HPP
