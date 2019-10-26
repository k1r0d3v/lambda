#ifndef LAMBDA_VARIABLE_HPP
#define LAMBDA_VARIABLE_HPP

#include "node.hpp"
#include "node_type.hpp"
#include "common.hpp"
#include "exception.hpp"
#include "type.hpp"

namespace ast
{
    class Variable : public Node
    {
    public:
        using Pointer = Node::PointerType<Variable>;

    public:
        explicit Variable(string name, Type::Pointer type)
                : Node(NodeType::Variable), mName(std::move(name)), mType(std::move(type)) { }

        const string &name() const
        {
            return mName;
        }

        Type::Pointer type() const {
            return mType;
        }

        Node::Pointer evaluate(Context &context) const override
        {
            throw ASTException("Variables are expected to be replaced, can not be evaluated");
        }

        Node::Pointer resolve(Context &context) const override
        {
            return Node::make<TypedValue>(Node::make<Variable>(mName, mType), mType);
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            if (a->nodeType() == NodeType::Variable)
            {
                auto variable = Node::cast<Variable>(a);
                if (variable->name() == mName)
                    return b;
            }

            return this->copy();
        }

        Node::Pointer copy() const override
        {
            return Node::make<Variable>(mName, mType);
        }

        string toString() const override
        {
            return mName;
        }

    private:
        string mName;
        Type::Pointer mType;
    };
}

#endif //LAMBDA_VARIABLE_HPP
