#ifndef LAMBDA_DRIVER_H
#define LAMBDA_DRIVER_H

#include <ast/ast.hpp>
#include <driver.hpp>
#include <utility>

#define YY_DRIVERDATA reinterpret_cast<ast::AST*>(driver->yyData())

namespace lambda
{
    class ASTDriver
    {
    public:
        static void parse(
                std::istream &stream,
                ast::AST *data,
                std::string name = "",
                bool traceScan = false,
                bool traceParse = false) {
            return yy::Driver::parse(stream, data, std::move(name), traceScan, traceParse);
        }
    };
}

#endif
