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

        bool equals(const Type::Pointer &t) const override;

        Type::Pointer typeof_dot(const Node::Pointer &b) override;

        Type::Pointer typeof_name(const string &name);

        string toString() const override;

    private:
        std::map<string, Type::Pointer> mElements;
    };
}


#endif //LAMBDA_REGISTER_TYPE_HPP
