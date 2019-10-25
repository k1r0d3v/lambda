#ifndef LAMBDA_AST_HPP
#define LAMBDA_AST_HPP

// All headers here
#include "exception.hpp"
#include "common.hpp"
#include "context.hpp"
#include "node.hpp"
#include "identifier.hpp"
#include "variable.hpp"
#include "abstraction.hpp"
#include "application.hpp"
#include "natural.hpp"
#include "boolean.hpp"
#include "natural_primitives.hpp"
#include "conditional.hpp"
#include "local_definition.hpp"
#include "unit.hpp"
#include "sequence.hpp"
#include "declaration.hpp"
//

namespace ast
{
    class AST
    {
    public:
        AST() = default;

        Node::Pointer evaluate(Context &context)
        {
            assert(mRoot != nullptr);
            return mRoot->resolve(context)->evaluate(context);
        }

        string toString() const
        {
            assert(mRoot != nullptr);
            return mRoot->toString();
        }

        Node::Pointer root() const
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
    };
}

#endif