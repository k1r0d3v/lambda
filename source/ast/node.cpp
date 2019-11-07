#include <ast/node.hpp>
#include <ast/node_visitor.hpp>

using namespace ast;

Node::Pointer Node::transform(const Node::Pointer &node, NodeVisitor *visitor)
{
    auto value = node->transform(visitor);
    if (value == nullptr)
        value = node;
    return value;
}

Node::Pointer Node::transform(const Node::Pointer &node, NodeVisitor &&visitor)
{
    NodeVisitor &tmp = visitor;
    return Node::transform(node, &tmp);
}