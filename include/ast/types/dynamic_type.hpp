#ifndef LAMBDA_DYNAMIC_TYPE_HPP
#define LAMBDA_DYNAMIC_TYPE_HPP

#include <ast/types/type.hpp>
#include <ast/types/type_kind.hpp>

namespace ast
{
    // TODO: Change to TopType
    class DynType : public Type
    {
    public:
        using Pointer = Type::PointerType<DynType>;

    public:
        static const Type::PointerType<DynType> INSTANCE;

    public:
        explicit DynType() : Type(TypeKind::Dyn) { }

        bool equals(const Type::Pointer &t) const override { return true; }

        string toString() const override { return "Top"; }
    };
}

#endif //LAMBDA_DYNAMIC_TYPE_HPP
