#ifndef LAMBDA_ASBTRACTION_TYPE_HPP
#define LAMBDA_ASBTRACTION_TYPE_HPP

#include "../common.hpp"
#include "type.hpp"

namespace ast
{
    class ArrowType : public Type
    {
    public:
        static const Type::PointerType<ArrowType> NAT_NAT;
        static const Type::PointerType<ArrowType> NAT_BOOL;

    public:
        explicit ArrowType(Type::Pointer left, Type::Pointer right)
                : mLeft(std::move(left)), mRight(std::move(right)) { }

        bool operator==(const Type &t) const override
        {
            auto at = dynamic_cast<const ArrowType*>(&t);
            if (at != nullptr)
                return Type::equals(mLeft, at->mLeft) && Type::equals(mRight, at->mRight);
            return false;
        }

        Type::Pointer left() const {
            return mLeft;
        }

        Type::Pointer right() const {
            return mRight;
        }

        string toString() const override
        {
            return "(" + mLeft->toString() + " -> " + mRight->toString() + ")";
        }

    private:
        Type::Pointer mLeft;
        Type::Pointer mRight;
    };
}

#endif //LAMBDA_CONSTANT_TYPE_HPP
