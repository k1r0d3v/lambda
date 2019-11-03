#ifndef LAMBDA_ASBTRACTION_TYPE_HPP
#define LAMBDA_ASBTRACTION_TYPE_HPP

#include "../common.hpp"
#include "type.hpp"
#include "dynamic_type.hpp"

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

        bool equals(const Type::Pointer &t) const override
        {
            auto at = Type::cast<ArrowType>(t);
            if (at != nullptr)
                return Type::equals(mLeft, at->mLeft) && Type::equals(mRight, at->mRight);
                // TODO: Move this from here
            else if (Type::cast<DynType>(t) != nullptr)
                return true;
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
            auto left = Type::cast<ArrowType>(mLeft);
            auto right = Type::cast<ArrowType>(mRight);

            auto os = std::ostringstream();

            if (left != nullptr)
                os << "(" << left->toString() << ")";
            else
                os << mLeft->toString();

            os << "->";

            if (right != nullptr)
                os << "(" << right->toString() << ")";
            else
                os << mRight->toString();

            return os.str();
        }

    private:
        Type::Pointer mLeft;
        Type::Pointer mRight;
    };
}

#endif //LAMBDA_CONSTANT_TYPE_HPP
