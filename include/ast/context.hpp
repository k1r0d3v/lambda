#ifndef LAMBDA_CONTEXT_HPP
#define LAMBDA_CONTEXT_HPP

#include <memory>
#include <map>
#include <vector>
#include "node.hpp"
#include "common.hpp"
#include "natural.hpp"

namespace ast
{
    class Context
    {
    private:
        using StrNodeMapType = std::map<string, Node::Pointer>;

    public:
        Context() = default;

        // env
        Node::Pointer setValue(const string& id, const Node::Pointer& value)
        {
            auto it = mMap.find(id);
            mMap[id] = value;

            if (it == mMap.end())
                return Node::Pointer();
            return it->second;
        }

        Node::Pointer getValue(const string& id) const
        {
            auto it = mMap.find(id);
            if (it == mMap.end())
                return Node::Pointer();
            return it->second;
        }

        Context copy() const
        {
            return Context();
        }

    private:
        StrNodeMapType mMap;
    };
}

#endif