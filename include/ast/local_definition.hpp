#ifndef LAMBDA_LOCAL_DEFINITION_HPP
#define LAMBDA_LOCAL_DEFINITION_HPP

#include "common.hpp"
#include "node.hpp"
#include "node_type.hpp"

namespace ast
{
    class LocalDefinition : public Node
    {
    public:
        using Pointer = Node::PointerType<LocalDefinition>;

    public:
        explicit LocalDefinition(Identifier::Pointer id, Node::Pointer value, Node::Pointer body)
                : Node(NodeType::LocalDefinition),
                  mId(std::move(id)),
                  mValue(std::move(value)),
                  mBody(std::move(body)) { }

        const Identifier::Pointer &id() const
        {
            return mId;
        }

        const Node::Pointer &value() const
        {
            return mValue;
        }

        const Node::Pointer &body() const
        {
            return mBody;
        }

        Node::Pointer evaluate(Context &context) const override
        {
            return mBody->replace(mId, mValue)->evaluate(context);
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            return Node::make<LocalDefinition>(mId, mValue->replace(a, b), mBody->replace(a, b));
        }

        Node::Pointer copy() const override
        {
            return Node::make<LocalDefinition>(mId, mValue, mBody);
        }

        string toString() const override
        {
            auto os = std::ostringstream();
            os << "let " << mId->toString() << " = " << mValue->toString() << " in " << mBody->toString();
            return os.str();
        }

    private:
        Identifier::Pointer mId;
        Node::Pointer mValue;
        Node::Pointer mBody;
    };
}

#endif //LAMBDA_LOCAL_DEFINITION_HPP
