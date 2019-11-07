#ifndef LAMBDA_NAT_TYPE_HPP
#define LAMBDA_NAT_TYPE_HPP

#include <ast/types/constant_type.hpp>
#include <ast/types/type_kind.hpp>

namespace ast
{
    class NatType : public ConstantType
    {
    public:
        static const Type::PointerType<NatType> INSTANCE;

    public:
        explicit NatType() : ConstantType(TypeKind::Nat, "Nat") { }
    };
}

#endif //LAMBDA_NAT_TYPE_HPP
