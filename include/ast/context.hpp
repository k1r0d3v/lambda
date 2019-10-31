#ifndef LAMBDA_CONTEXT_HPP
#define LAMBDA_CONTEXT_HPP

#include <memory>
#include <map>
#include <vector>
#include "node.hpp"
#include "common.hpp"
#include "ast/types/type.hpp"

namespace ast
{
    class Context
    {
    private:
        using StrNodeMapType = std::map<string, Node::Pointer>;

    public:
        Context() = default;

        Node::Pointer setValue(const string& id, const Node::Pointer& value)
        {
            if (value != nullptr)
            {
                auto it = mMap.find(id);
                mMap[id] = value;

                if (it == mMap.end())
                    return Node::Pointer();
                return it->second;
            }
            else
            {
                mMap.erase(id);
                return Node::Pointer();
            }
        }

        Node::Pointer getValue(const string& id) const
        {
            auto it = mMap.find(id);
            if (it == mMap.end())
                return Node::Pointer();
            return it->second;
        }

        void stackPush(Node::Pointer value)
        {
            mStack.push_back(value);
        }

        void stackPop()
        {
            mStack.erase(mStack.end());
        }

        Node::Pointer stackAtIndex(size_t index) const
        {
            return mStack[index];
        }

        size_t stackSize() const {
            return mStack.size();
        }

    private:
        StrNodeMapType mMap;
        std::vector<Node::Pointer> mStack;
    };
}

#endif