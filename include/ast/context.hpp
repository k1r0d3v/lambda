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

        // stack

        // env
        Node::Pointer getValueOfId(const string &name)
        {
            return Node::Pointer();
        }


        Context copy() const
        {
            return Context();
        }

        static Context empty()
        {
            return Context();
        }

    private:
    };
}

#endif