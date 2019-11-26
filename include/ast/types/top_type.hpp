#ifndef LAMBDA_TOP_TYPE_HPP
#define LAMBDA_TOP_TYPE_HPP

#include <ast/types/type.hpp>
#include <ast/types/type_kind.hpp>

namespace ast
{
    class TopType : public Type
    {
    public:
        using Pointer = Type::PointerType<TopType>;

    public:
        static const Type::PointerType<TopType> INSTANCE;

    public:
        explicit TopType() : Type(TypeKind::Top) { }

        bool isTypeOf(const Type::Pointer &t) const override { return t->kind() == TypeKind::Top; }

        bool isSubtypeOf(const Type::Pointer &t) const override { return false; }

        string toString() const override { return "Top"; }
    };
}

#endif //LAMBDA_TOP_TYPE_HPP
