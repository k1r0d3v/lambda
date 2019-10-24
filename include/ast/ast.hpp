#ifndef LAMBDA_AST_HPP
#define LAMBDA_AST_HPP

// All headers here
#include "common.hpp"
#include "exception.hpp"
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
#include "unit.hpp"
#include "sequence.hpp"
//

namespace ast
{
    class AST
    {
    public:
        AST() = default;

        explicit AST(Node::Pointer root)
                : mRoot(std::move(root)) { }

        AST(const Context context, Node::Pointer root)
                : mRoot(std::move(root)), mContext(context) { }

        Node::Pointer evaluate(const Context &context) const
        {
            return mRoot->evaluate(context);
        }

        Node::Pointer evaluate() const
        {
            return evaluate(Context::empty());
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
            mRoot = std::move(root);
        }

        bool empty() const
        {
            return mRoot == nullptr;
        }

    private:
        Node::Pointer mRoot;
        Context mContext = Context::empty();
    };
}

#endif