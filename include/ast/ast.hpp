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
#include "string_constant.hpp"
#include "ast/types/type.hpp"
#include "ast/types/top_type.hpp"
#include "ast/types/arrow_type.hpp"
#include "ast/types/constant_type.hpp"
#include "ast/types/tuple_type.hpp"
#include "ast/types/register_type.hpp"
#include "ast/types/bool_type.hpp"
#include "ast/types/nat_type.hpp"
#include "ast/types/str_type.hpp"
#include "ast/types/unit_type.hpp"
#include "types/undefined_type.hpp"
#include "print_primitive.hpp"
#include "ascription.hpp"
#include "tuple.hpp"
#include "register.hpp"
#include "operator_dot.hpp"
#include "fix.hpp"
#include "alias.hpp"
//

namespace ast
{
    class AST
    {
    public:
        AST() = default;


        Type::Pointer typecheck(TypeContext &context)
        {
            assert(mRoot != nullptr);
            return mRoot->typecheck(context);
        }

        Node::Pointer evaluate(Context &context)
        {
            assert(mRoot != nullptr);
            return mRoot->evaluate(context);
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
