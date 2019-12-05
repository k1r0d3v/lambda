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
     */
    class Node
    {
    public:
        using Pointer = std::shared_ptr<Node>;

        template<typename T>
        using PointerType = std::shared_ptr<T>;

        /**
         * Cast the Node::Pointer to a T class subtype the Node
         * @return the cast value or nullptr if the class is not T
         * */
        template<typename T>
        static PointerType<T> cast(Node::Pointer ref)
        {
            return std::dynamic_pointer_cast<T>(ref);
        }

        /**
         * Create a instance of T class with arguments
         * @return the object create
         **/
        template<typename T, typename ...Args>
        static PointerType<T> make(Args &&... args)
        {
            return std::make_shared<T>(std::forward<Args>(args)...);
        }

        /**
         *
         * @return
         */
        static Node::Pointer transform(const Node::Pointer &node, NodeVisitor *visitor);

        /**
         *
         * @return
         */
        static Node::Pointer transform(const Node::Pointer &node, NodeVisitor &&visitor);

    public:
        /**
         * The constructor receive a number of enum NodeKind
         **/
        explicit Node(int kind) : mKind(kind) { }

        /**
         * This function is responsible for solving the terms and context dependencies
         * @return the final value of term after evaluate
         */
        virtual Node::Pointer evaluate(Context &context) const = 0;

        /**
         * This function is responsible for check the types terms and use context dependencies
         * @return type of term
         */
        virtual Type::Pointer typecheck(TypeContext &context) = 0;

        /**
         *
         * @return nullptr if this node is not replaced (children can be replaced even so) else the replace node
         */
        virtual Node::Pointer transform(NodeVisitor *visitor) = 0;

        /**
         * Transform the values of class in string to view in output
         * @return string
         */
        virtual string toString() const = 0;

        /**
         * Duplicate instance of this object
         * @return Node::Pointer
         **/
        virtual Node::Pointer copy() const = 0;

        /**
         * Return a enum value of NodeKind who is the type of Node
         * @return int
         */
        int kind() const { return mKind; }

    private:
        const int mKind;
    };
}

#endif