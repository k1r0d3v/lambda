#ifndef LAMBDA_ASBTRACTION_TYPE_HPP
#define LAMBDA_ASBTRACTION_TYPE_HPP

#include "common.hpp"
#include "node.hpp"
#include "node_type.hpp"

namespace ast
{
    class AbstractionType : public Type
    {
    public:
        explicit AbstractionType(Type::Pointer left, Type::Pointer right)
                : mLeft(std::move(left)), mRight(std::move(right)) { }

        bool equals(Type::Pointer t) const override
        {
            auto at = Type::cast<AbstractionType>(t);
            if (at != nullptr)
                return mLeft->equals(at->mLeft) && mRight->equals(at->mRight);
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
