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

        Node::Pointer setValue(const string& id, const Node::Pointer& value);

        Node::Pointer getValue(const string& id) const;

    private:
        std::map<string, Node::Pointer> mMap;
    };
}

#endif