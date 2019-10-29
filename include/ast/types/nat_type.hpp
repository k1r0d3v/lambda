#ifndef LAMBDA_NAT_TYPE_HPP
#define LAMBDA_NAT_TYPE_HPP

#include "../common.hpp"
#include "type.hpp"
#include "constant_type.hpp"

namespace ast
{
    class NatType : public ConstantType
    {
    public:
        static const Type::PointerType<NatType> NAT;

    public:
        explicit NatType() : ConstantType("Nat") { }
    };
}

#endif //LAMBDA_NAT_TYPE_HPP
