#ifndef LAMBDA_NODE_HPP
#define LAMBDA_NODE_HPP

#include "common.hpp"

namespace ast
{
    class Context; // Forward declaration of Context

    // TODO: Add character offset of the node
    /**
     *
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
        explicit Node(int type) : mType(type) { }

        /**
         * @param context A given context
         * @return Result of the evaluation, this value can be a real tree node or a copy
         */
        virtual Node::Pointer evaluate(const Context &context) const = 0;

        /**
         * Replaces recursively the node @param{a} in the tree with by the node @param{b}
         *
         * Note: The replacement is implementation dependent,
         * implementors can impose restrictions over the node to replace or be replaced
         *
         * @return A copy of the tree with the nodes replaced
         */
        virtual Node::Pointer replace(Node::Pointer a, Node::Pointer b) const = 0;

        /**
         * @return String representation of this node and children
         */
        virtual string toString() const = 0;

        /**
         * @return A recursive copy of this node
         */
        virtual Node::Pointer copy() const = 0;

        /**
         *
         * @return Integer value representing the node type
         */
        int type() const
        {
            return mType;
        }

    private:
        const int mType;
    };
}

#endif