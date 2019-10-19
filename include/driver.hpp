#ifndef LAMBDA_DRIVER_HPP
#define LAMBDA_DRIVER_HPP

#include <istream>
#include <utility>
#include <lambda/parser.hpp>


// Give to flex the prototype of yylex we want
#define YY_DECL lambda::Parser::symbol_type yylex(lambda::DriverBase *driver)

namespace lambda
{
    class DriverBase
    {
    public:
        explicit DriverBase(std::istream &stream, std::string name = "")
                : mStream(stream), mStreamName(std::move(name)) {}

        int parse();

        void setTraceScanning(bool value)
        {
            mTraceScanning = value;
        }

        void setTraceParsing(bool value)
        {
            mTraceParsing = value;
        }

        lambda::location &location()
        {
            return mLocation;
        }

    private:
        bool mTraceScanning = false;
        bool mTraceParsing = false;
        lambda::location mLocation;
        std::string mStreamName;
        std::istream &mStream;
    };
}

//
// Declare it for the parser's sake.
//
YY_DECL;

#endif
