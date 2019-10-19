#ifndef LAMBDA_AST_HPP
#define LAMBDA_AST_HPP

// All headers here
#include "context.hpp"
#include "node.hpp"
#include "identifier.hpp"
#include "abstraction.hpp"
#include "application.hpp"
#include "common.hpp"
//

namespace ast
{
    class AST
    {
    public:
        AST() = default;

        AST(Node::Reference root)
                : mRoot(root) {}

        AST(const Context context, Node::Reference root)
                : mRoot(root), mContext(context) {}

        Node::Reference evaluate(const Context &context) const
        {
            return mRoot->evaluate(context)->copy();
        }

        Node::Reference evaluate() const
        {
            return evaluate(Context::empty())->copy();
        }

        string toString() const
        {
            return mRoot->toString();
        }

        Node::Reference root()
        {
            return mRoot;
        }

        void setRoot(Node::Reference root)
        {
            mRoot = root;
        }

        bool empty() const
        {
            return mRoot == nullptr;
        }

    private:
        Node::Reference mRoot;
        Context mContext{Context::empty()};
    };
}

#endif