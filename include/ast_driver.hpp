#ifndef LAMBDA_DRIVER_H
#define LAMBDA_DRIVER_H

#include <ast/ast.hpp>
#include <driver.hpp>

#ifdef YYDRIVERDATA
#undef YYDRIVERDATA
#endif

#define YYDRIVERDATA (reinterpret_cast<AstDriver*>(driver)->yyData)

namespace lambda
{
    class AstDriver : public lambda::DriverBase
    {
    public:
        // Allow Driver::data from parser.y
        friend class lambda::Parser;

    public:
        AstDriver(ast::AST *data, std::istream &stream, std::string name = "")
                : lambda::DriverBase(stream, std::move(name)), yyData(data) { }

    private:
        ast::AST *yyData;
    };
}

#endif
