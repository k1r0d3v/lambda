#ifndef FLEXBISON_DRIVER_HPP
#define FLEXBISON_DRIVER_HPP

#include <istream>
#include <utility>
#include <string>
#include <vector>
#include <parser.hpp>

// Give to flex the prototype of yylex we want
#define YY_DECL yy::Parser::symbol_type yylex(yy::Driver *driver)

namespace yy
{
    class Driver
    {
    private:
        explicit Driver() = default;

    public:
        static yy::Driver parse(
                std::istream &stream,
                void *data,
                std::string filename = "",
                bool traceScan = false,
                bool traceParse = false);

    public:
        void *yyData() { return mData; }

        yy::location &yyLocation() { return mLocation; }

        void addError(const std::string &error)
        {
            mErrors.push_back(error);
        }

        const std::vector<std::string> &errors()
        {
            return mErrors;
        }

    private:
        yy::location mLocation;
        std::vector<std::string> mErrors;
        void *mData = nullptr;
    };
}

//
// Declare it for the parser's sake.
//
YY_DECL;

#endif
