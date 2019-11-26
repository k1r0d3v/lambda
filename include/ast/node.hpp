#ifndef LAMBDA_NODE_HPP
#define LAMBDA_NODE_HPP

#include <ast/common.hpp>
#include <ast/types/type.hpp>
#include <ast/types/type_context.hpp>

namespace ast
{
    class Context;
    class NodeVisitor;

    /**
     *  Base class for nodes
     *
     *  TODO: Add character offset to the node
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

        static Node::Pointer transform(const Node::Pointer &node, NodeVisitor *visitor);

        static Node::Pointer transform(const Node::Pointer &node, NodeVisitor &&visitor);

    public:
        explicit Node(int kind) : mKind(kind) { }

        virtual Node::Pointer evaluate(Context &context) const = 0;

        virtual Type::Pointer typecheck(TypeContext &context) = 0;

        /**
         *
         * @return nullptr if this node is not replaced (children can be replaced even so) else the replace node
         */
        virtual Node::Pointer transform(NodeVisitor *visitor) = 0;

        virtual string toString() const = 0;

        virtual Node::Pointer copy() const = 0;

        int kind() const { return mKind; }

    private:
        const int mKind;
    };
}

#endif