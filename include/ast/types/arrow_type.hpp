#ifndef LAMBDA_ASBTRACTION_TYPE_HPP
#define LAMBDA_ASBTRACTION_TYPE_HPP

#include <ast/types/type.hpp>
#include <ast/types/type_kind.hpp>

namespace ast
{
    class ArrowType : public Type
    {
    public:
        static const Type::PointerType<ArrowType> NAT_NAT;
        static const Type::PointerType<ArrowType> NAT_BOOL;

    public:
        explicit ArrowType(Type::Pointer left, Type::Pointer right);

        Type::Pointer left() const { return mLeft; }

        Type::Pointer right() const { return mRight; }

        bool equals(const Type::Pointer &t) const override;

        string toString() const override;

    private:
        Type::Pointer mLeft;
        Type::Pointer mRight;
    };
}

#endif //LAMBDA_CONSTANT_TYPE_HPP
