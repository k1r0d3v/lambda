#ifndef LAMBDA_PATTERN_HPP
#define LAMBDA_PATTERN_HPP


#include "common.hpp"
#include "node_type.hpp"
#include "node.hpp"

namespace ast
{
    class Pattern : public Node
    {
    public:
        using Pointer = Node::PointerType<Pattern>;
        using MatchResult = list<pair<string, Node::Pointer>>;
        using TypecheckMatchResult = list<pair<string, Type::Pointer>>;
        using MatchIdenfiers = list<string>;

    public:
        explicit Pattern(int type) : Node(type) { }

        virtual MatchIdenfiers matchIdentifiers() const = 0;

        virtual MatchResult match(const Node::Pointer &value, Context &context) const = 0;

        virtual TypecheckMatchResult typecheckMatch(const Type::Pointer &type, TypeContext &context) const = 0;
    };
}

#endif //LAMBDA_PATTERN_HPP
