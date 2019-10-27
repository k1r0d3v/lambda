#ifndef LAMBDA_DECLARATION_HPP
#define LAMBDA_DECLARATION_HPP

#include "node.hpp"
#include "node_type.hpp"
#include "common.hpp"
#include "identifier.hpp"

namespace ast
{
    class Declaration : public Node
    {
    public:
        using Pointer = Node::PointerType<Declaration>;

    public:
        explicit Declaration(Identifier::Pointer id, Node::Pointer value)
                : Node(NodeType::Declaration), mId(std::move(id)), mValue(std::move(value)) { }

        const Identifier::Pointer &id() const {
            return mId;
        }

        const Node::Pointer &value() const {
            return mValue;
        }

        Node::Pointer evaluate(Context &context) const override
        {
            auto evaluatedValue = mValue->evaluate(context);
            context.setValue(mId->name(), evaluatedValue);
            return evaluatedValue;
        }

        void resolve(Context &context) override
        {
            mValue->resolve(context);
            this->setType(mValue->type());
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            return Node::make<Declaration>(mId, mValue->replace(a, b));
        }

        Node::Pointer copy() const override
        {
            auto copy = Node::make<Declaration>(mId, mValue);
            copy->setType(this->type());
            return copy;
        }

        string toString() const override
        {
            auto os = std::ostringstream();
            os << "let " << mId->toString() << " = " << mValue->toString() << ";";
            return os.str();
        }

    private:
        Identifier::Pointer mId;
        Node::Pointer mValue;
    };
}


#endif //LAMBDA_DECLARATION_HPP
