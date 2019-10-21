#ifndef FLEXBISON_DRIVER_HPP
#define FLEXBISON_DRIVER_HPP

#include <istream>
#include <utility>
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
        static void parse(
                std::istream &stream,
                void *data,
                std::string name = "",
                bool traceScan = false,
                bool traceParse = false);

    public:
        void *yyData() { return mData; }

        yy::location &yyLocation() { return mLocation; }

    private:
        yy::location mLocation;
        void *mData = nullptr;
    };
}

//
// Declare it for the parser's sake.
//
YY_DECL;

#endif
