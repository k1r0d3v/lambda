#ifndef LAMBDA_AST_HPP
#define LAMBDA_AST_HPP

// All headers here
#include "common.hpp"
#include "context.hpp"
#include "node.hpp"
#include "identifier.hpp"
#include "abstraction.hpp"
#include "application.hpp"
#include "natural.hpp"
#include "boolean.hpp"
#include "natural_primitives.hpp"
#include "conditional.hpp"
#include "local_definition.hpp"
//

namespace ast
{
    class AST
    {
    public:
        AST() = default;

        AST(Node::Pointer root)
                : mRoot(root) {}

        AST(const Context context, Node::Pointer root)
                : mRoot(root), mContext(context) {}

        Node::Pointer evaluate(const Context &context) const
        {
            return mRoot->evaluate(context)->copy();
        }

        Node::Pointer evaluate() const
        {
            return evaluate(Context::empty())->copy();
        }

        string toString() const
        {
            return mRoot->toString();
        }

        Node::Pointer root()
        {
            return mRoot;
        }

        void setRoot(Node::Pointer root)
        {
            mRoot = root;
        }

        bool empty() const
        {
            return mRoot == nullptr;
        }

    private:
        Node::Pointer mRoot;
        Context mContext{Context::empty()};
    };
}

#endif