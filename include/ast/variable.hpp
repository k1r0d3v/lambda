#ifndef LAMBDA_VARIABLE_HPP
#define LAMBDA_VARIABLE_HPP

#include "node.hpp"
#include "node_type.hpp"
#include "common.hpp"
#include "exception.hpp"

namespace ast
{
    class Variable : public Node
    {
    public:
        using Pointer = Node::PointerType<Variable>;

    public:
        explicit Variable(string name)
                : Node(NodeType::Variable), mName(std::move(name)) { }

        const string &name() const
        {
            return mName;
        }

        Node::Pointer evaluate(Context &context) const override
        {
            throw ASTException("Variables are expected to be replaced, can not be evaluated");
        }

        Node::Pointer resolve(const Context &context) const override
        {
            return this->copy();
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            if (a->type() == NodeType::Variable)
            {
                auto variable = Node::cast<Variable>(a);
                if (variable->name() == mName)
                    return b;
            }

            return this->copy();
        }

        Node::Pointer copy() const override
        {
            return Node::make<Variable>(mName);
        }

        string toString() const override
        {
            return mName;
        }

    private:
        string mName;
    };
}

#endif //LAMBDA_VARIABLE_HPP
