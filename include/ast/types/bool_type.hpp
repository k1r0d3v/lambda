#ifndef LAMBDA_BOOL_TYPE_HPP
#define LAMBDA_BOOL_TYPE_HPP

#include <ast/types/constant_type.hpp>
#include <ast/types/type_kind.hpp>

namespace ast
{
    class BoolType : public ConstantType
    {
    public:
        static const Type::PointerType<BoolType> INSTANCE;

    public:
        explicit BoolType() : ConstantType(TypeKind::Bool, "Bool") { }
    };
}

#endif //LAMBDA_BOOL_TYPE_HPP
