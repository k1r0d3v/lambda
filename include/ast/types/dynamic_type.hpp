#ifndef LAMBDA_DYNAMIC_TYPE_HPP
#define LAMBDA_DYNAMIC_TYPE_HPP

#include "../common.hpp"
#include "type.hpp"

namespace ast
{
    class DynType : public Type
    {
    public:
        using Pointer = Type::PointerType<DynType>;

    public:
        static const Type::PointerType<DynType> INSTANCE;

    public:
        bool equals(const Type::Pointer &t) const override
        {
            return true;
        }

        string toString() const override
        {
            return "Dyn";
        }
    };
}

#endif //LAMBDA_DYNAMIC_TYPE_HPP
