#ifndef LAMBDA_UNIT_TYPE_HPP
#define LAMBDA_UNIT_TYPE_HPP

#include "../common.hpp"
#include "type.hpp"
#include "constant_type.hpp"

namespace ast
{
    class UnitType : public ConstantType
    {
    public:
        static const Type::PointerType<UnitType> UNIT;

    public:
        explicit UnitType() : ConstantType("Unit") { }
    };
}


#endif //LAMBDA_UNIT_TYPE_HPP
