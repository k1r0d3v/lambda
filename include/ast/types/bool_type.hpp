#ifndef LAMBDA_BOOL_TYPE_HPP
#define LAMBDA_BOOL_TYPE_HPP

#include "../common.hpp"
#include "type.hpp"
#include "constant_type.hpp"

namespace ast
{
    class BoolType : public ConstantType
    {
    public:
        static const Type::PointerType<BoolType> INSTANCE;

    public:
        explicit BoolType() : ConstantType("Bool") { }
    };
}

#endif //LAMBDA_BOOL_TYPE_HPP
