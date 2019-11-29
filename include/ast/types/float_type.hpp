#ifndef LAMBDA_NAT_TYPE_HPP
#define LAMBDA_NAT_TYPE_HPP

#include <ast/types/constant_type.hpp>
#include <ast/types/type_kind.hpp>

namespace ast
{
    class FloatType : public ConstantType
    {
    public:
        static const Type::PointerType<FloatType> INSTANCE;

    public:
        explicit FloatType() : ConstantType(TypeKind::Float, "Float") { }
    };
}

#endif //LAMBDA_NAT_TYPE_HPP
