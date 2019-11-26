#ifndef LAMBDA_CONSTANT_TYPE_HPP
#define LAMBDA_CONSTANT_TYPE_HPP

#include <ast/types/type.hpp>

namespace ast
{
    class ConstantType : public Type
    {
    public:
        using Pointer = Type::PointerType<ConstantType>;

    public:
        explicit ConstantType(int kind, const string &name);

        const string &name() const { return mName; }

    public: // Type
        bool isTypeOf(const Type::Pointer &t) const override;

        bool isSubtypeOf(const Type::Pointer &t) const override ;

        string toString() const override;

    private:
        string mName;
    };
}

#endif //LAMBDA_CONSTANT_TYPE_HPP
