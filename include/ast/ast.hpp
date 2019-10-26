#ifndef LAMBDA_AST_HPP
#define LAMBDA_AST_HPP

// All headers here
#include "common.hpp"
#include "context.hpp"
#include "node.hpp"
#include "exception.hpp"
#include "identifier.hpp"
#include "variable.hpp"
#include "abstraction.hpp"
#include "application.hpp"
#include "natural_constant.hpp"
#include "boolean_constant.hpp"
#include "natural_primitives.hpp"
#include "conditional.hpp"
#include "local_definition.hpp"
#include "unit.hpp"
#include "sequence.hpp"
#include "declaration.hpp"
#include "type.hpp"
#include "abstraction_type.hpp"
#include "constant_type.hpp"
#include <ast/typed_value.hpp>
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
            auto resolved = Node::cast<TypedValue>(mRoot->resolve(context));
            return Node::make<TypedValue>(resolved->evaluate(context), resolved->type());
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
