#ifndef LAMBDA_NODE_HPP
#define LAMBDA_NODE_HPP

#include "common.hpp"

namespace ast
{
    class Context; // Forward declaration of Context

    // TODO: Add character offset of the node
    class Node
    {
    public:
        using Reference = std::shared_ptr<Node>;

        template<typename T>
        static std::shared_ptr<T> castRef(Node::Reference ref)
        {
            return std::dynamic_pointer_cast<T>(ref);
        }

        template<typename T, typename ...Args>
        static std::shared_ptr<T> make(Args &&... args)
        {
            return std::make_shared<T>(std::forward<Args>(args)...);
        }

    public:
        Node(int type) : mType(type) {}

        /**
         * @param context A given context
         * @return Result of the evaluation, this value can be a real tree node or a copy
         */
        virtual Reference evaluate(const Context &context) const = 0;

        /**
         * @return String representation of this node and children
         */
        virtual string toString() const = 0;

        /**
         * @return A recursive copy of this node
         */
        virtual Node::Reference copy() const = 0;

        /**
         * @return Try to cast this node to the type @tparam{T} else empty reference
         */
        template<typename T>
        std::shared_ptr<T> copyCast() const
        {
            return Node::castRef<T>(this->copy());
        }

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