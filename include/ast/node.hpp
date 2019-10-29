#ifndef LAMBDA_NODE_HPP
#define LAMBDA_NODE_HPP

#include <ast/types/type_context.hpp>
#include "common.hpp"

namespace ast
{
    class Context; // Forward declaration of Context

    // TODO: Add character offset to the node
    /**
     *  Abstract Syntax Tree Node
     */
    class Node
    {
    public:
        using Pointer = std::shared_ptr<Node>;

        template<typename T>
        using PointerType = std::shared_ptr<T>;

        template<typename T>
        static PointerType<T> cast(Node::Pointer ref)
        {
            return std::dynamic_pointer_cast<T>(ref);
        }

        template<typename T, typename ...Args>
        static PointerType<T> make(Args &&... args)
        {
            return std::make_shared<T>(std::forward<Args>(args)...);
        }

        template<typename T>
        static PointerType<T> makeNoDeletablePtr(T *ptr)
        {
            return PointerType<T>(PointerType<T>{}, ptr);
        }

    public:
        explicit Node(int nodeType) : mNodeType(nodeType) { }

        /**
         *
         */
        virtual Node::Pointer evaluate(Context &context) const = 0;

        /**
         *
         */
        virtual Node::Pointer freeze(Context &context) const = 0;

        /**
         *
         */
        virtual Type::Pointer typecheck(TypeContext &context) const = 0;

        /**
         * @return String representation of this node and children.
         */
        virtual string toString() const = 0;

        /**
         * @return A copy of this node tree.
         */
        virtual Node::Pointer copy() const = 0;

        /**
         *
         * @return Integer value representing the node type in the tree.
         */
        int nodeType() const
        {
            return mNodeType;
        }

    private:
        const int mNodeType;
    };
}

#endif