#ifndef LAMBDA_TUPLE_TYPE_HPP
#define LAMBDA_TUPLE_TYPE_HPP

#include <ast/types/type.hpp>
#include <ast/types/operable_type.hpp>
#include <ast/types/type_kind.hpp>

namespace ast
{
    class TupleType : public OperableType
    {
    public:
        explicit TupleType(vector<Type::Pointer> types);

        template<typename Iterator>
        TupleType(Iterator begin, Iterator end) : OperableType(TypeKind::Tuple) {
            mElements = {begin, end};
        }

        const vector<Type::Pointer> &elements() const { return mElements; }

        void resolve(TypeContext &context) override;

        bool isTypeOf(const Type::Pointer &t) const override;

        bool isSubtypeOf(const Type::Pointer &t) const override;

        Type::Pointer typeOfDot(const Node::Pointer &b) override;

        Type::Pointer typeofIndex(int index);

        string toString() const override;

    private:
        vector<Type::Pointer> mElements;
    };
}


#endif //LAMBDA_TUPLE_TYPE_HPP
