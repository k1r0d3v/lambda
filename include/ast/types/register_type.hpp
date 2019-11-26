#ifndef LAMBDA_REGISTER_TYPE_HPP
#define LAMBDA_REGISTER_TYPE_HPP

#include <ast/types/type.hpp>
#include <ast/types/operable_type.hpp>

namespace ast
{
    class RegisterType : public OperableType
    {
    public:
        explicit RegisterType(std::map<string, Type::Pointer> types);

        const std::map<string, Type::Pointer> &elements() const { return mElements; }

        void resolve(TypeContext &context) override;

        bool isTypeOf(const Type::Pointer &t) const override;

        bool isSubtypeOf(const Type::Pointer &t) const override;

        Type::Pointer typeOfDot(const Node::Pointer &b) override;

        Type::Pointer typeOfName(const string &name);

        string toString() const override;

    private:
        std::map<string, Type::Pointer> mElements;
    };
}


#endif //LAMBDA_REGISTER_TYPE_HPP
