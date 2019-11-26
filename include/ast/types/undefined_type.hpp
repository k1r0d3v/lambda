#ifndef LAMBDA_UNDEFINEDTYPE_HPP
#define LAMBDA_UNDEFINEDTYPE_HPP

#include <ast/types/constant_type.hpp>
#include <ast/types/type_kind.hpp>

namespace ast
{
    class UndefinedType : public ConstantType
    {
    public:
        explicit UndefinedType(const string &name) : ConstantType(TypeKind::Undefined, name) { }

        void resolve(TypeContext &context) override;

        bool isTypeOf(const Type::Pointer &t) const override;

        bool isSubtypeOf(const Type::Pointer &t) const override;

        string toString() const override;

    private:
        Type::Pointer mType = nullptr;
    };
}


#endif //LAMBDA_UNDEFINEDTYPE_HPP
