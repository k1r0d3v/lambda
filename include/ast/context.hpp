#ifndef LAMBDA_CONTEXT_HPP
#define LAMBDA_CONTEXT_HPP

#include <memory>
#include <map>
#include <vector>
#include "node.hpp"
#include "common.hpp"

namespace ast
{
    class Context
    {
    private:
        using StrNodeMapType = std::map<string, Node::Pointer>;
        using StrStrMapType = std::map<string, string>;

    public:
        Context() = default;

        Context(const StrNodeMapType &values)
                : mValues(values) {}

        Node::Pointer get(const StrNodeMapType::key_type &name) const
        {
            auto it = mValues.find(name);
            if (it != mValues.end())
                return it->second;

            return Node::Pointer();
        }

        void set(const StrNodeMapType::key_type &name, StrNodeMapType::mapped_type value)
        {
            mValues[name] = value;
        }

        StrNodeMapType::iterator values()
        {
            return mValues.begin();
        }

        Context copy() const
        {
            return Context(mValues);
        }

        static Context empty()
        {
            return Context();
        }

    private:
        StrNodeMapType mValues;
    };
}

#endif