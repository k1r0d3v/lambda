#include <ast/context.hpp>

using namespace ast;

Node::Pointer Context::setValue(const string& id, const Node::Pointer& value)
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

Node::Pointer Context::getValue(const string& id) const
{
    auto it = mMap.find(id);
    if (it == mMap.end())
        return Node::Pointer();
    return it->second;
}