#ifndef LAMBDA_LIST_TYPE_HPP
#define LAMBDA_LIST_TYPE_HPP

#include "type_kind.hpp"
#include "constant_type.hpp"

namespace ast
{
    class ListType : public ConstantType
    {
    public:
        explicit ListType(Type::Pointer type);

        const Type::Pointer &type() const { return mType; }

        void resolve(TypeContext &context) override;

        bool isTypeOf(const Type::Pointer &t) const override;

        bool isSubtypeOf(const Type::Pointer &t) const override;

        string toString() const override;

    private:
        Type::Pointer mType;
    };
}
#endif //LAMBDA_LIST_TYPE_HPP
