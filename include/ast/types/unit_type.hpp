#ifndef LAMBDA_UNIT_TYPE_HPP
#define LAMBDA_UNIT_TYPE_HPP

#include <ast/types/constant_type.hpp>
#include <ast/types/type_kind.hpp>

namespace ast
{
    class UnitType : public ConstantType
    {
    public:
        static const Type::PointerType<UnitType> INSTANCE;

    public:
        explicit UnitType() : ConstantType(TypeKind::Unit, "Unit") { }
    };
}


#endif //LAMBDA_UNIT_TYPE_HPP
