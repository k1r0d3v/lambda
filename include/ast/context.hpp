#ifndef LAMBDA_CONTEXT_HPP
#define LAMBDA_CONTEXT_HPP

#include <ast/common.hpp>
#include <ast/node.hpp>

namespace ast
{
    class Context
    {
    public:
        Context() = default;

        /**
         * Set the value of id to the new value passed
         *  @return the value or a Node::Pointer if the id not exist
         * */
        Node::Pointer setValue(const string& id, const Node::Pointer& value);
        /**
         * Get the value of id
         *  @return the value or a Node::Pointer if the id not exist
         * */
        Node::Pointer getValue(const string& id) const;

    private:
        std::map<string, Node::Pointer> mMap;
    };
}

#endif