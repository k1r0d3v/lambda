#ifndef LAMBDA_CONTEXT_HPP
#define LAMBDA_CONTEXT_HPP

#include <memory>
#include <map>
#include <vector>
#include "node.hpp"
#include "common.hpp"
#include "type.hpp"

namespace ast
{
    class Context
    {
    private:
        using StrNodeMapType = std::map<string, Node::Pointer>;
        using StrTypeMapType = std::map<string, Type::Pointer>;

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

        Type::Pointer setType(const string& id, const Type::Pointer& value)
        {
            auto it = mTypesMap.find(id);
            mTypesMap[id] = value;

            if (it == mTypesMap.end())
                return Type::Pointer();
            return it->second;
        }

        Type::Pointer getType(const string& id) const
        {
            auto it = mTypesMap.find(id);
            if (it == mTypesMap.end())
                return Type::Pointer();
            return it->second;
        }

        Context copy() const
        {
            return Context();
        }

    private:
        StrNodeMapType mMap;
        StrTypeMapType mTypesMap;
    };
}

#endif