#ifndef LAMBDA_ASCRIPTION_HPP
#define LAMBDA_ASCRIPTION_HPP

#include "node.hpp"
#include "node_type.hpp"
#include "common.hpp"
#include "ast/types/type.hpp"
#include "exception.hpp"

namespace ast
{
    class Ascription : public Node
    {
    public:
        using Pointer = Node::PointerType<Ascription>;

    public:
        explicit Ascription(Node::Pointer node, Type::Pointer type)
                : Node(NodeType::Ascription), mNode(std::move(node)), mType(std::move(type))
        { }

        Node::Pointer evaluate(const Node::Pointer &self, Context &context) const override
        {
            return mNode->evaluate(mNode, context);
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            auto nodeType = mNode->typecheck(context);
            if (Type::distinct(nodeType, mType))
                throw TypeException("Cannot convert \'" + nodeType->toString() + "\' to \'" + mType->toString() + "\'");
            return nodeType;
        }

        Node::Pointer copy() const override
        {
            return Node::make<Ascription>(mNode, mType);
        }

        string toString() const override
        {
            return mNode->toString();
        }

    private:
        Node::Pointer mNode;
        Type::Pointer mType;
    };
}

#endif //LAMBDA_ASCRIPTION_HPP
