#ifndef LAMBDA_TUPLE_TYPE_HPP
#define LAMBDA_TUPLE_TYPE_HPP

#include <ast/types/type.hpp>
#include <ast/types/operable_type.hpp>

namespace ast
{
    class TupleType : public OperableType
    {
    public:
        explicit TupleType(list<Type::Pointer> types);

        const list<Type::Pointer> &elements() const { return mElements; }

        bool equals(const Type::Pointer &t) const override;

        Type::Pointer typeof_dot(const Node::Pointer &b) override;

        Type::Pointer typeof_index(int index);

        string toString() const override;

    private:
        list<Type::Pointer> mElements;
    };
}


#endif //LAMBDA_TUPLE_TYPE_HPP
