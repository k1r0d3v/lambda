#ifndef LAMBDA_STR_TYPE_HPP
#define LAMBDA_STR_TYPE_HPP

#include "../common.hpp"
#include "type.hpp"
#include "constant_type.hpp"

namespace ast
{
    class StrType : public ConstantType
    {
    public:
        static const Type::PointerType<StrType> INSTANCE;

    public:
        explicit StrType() : ConstantType("Str") { }
    };
}

#endif //LAMBDA_STR_TYPE_HPP
