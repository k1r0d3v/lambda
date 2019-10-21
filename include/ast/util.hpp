#ifndef LAMBDA_UTIL_HPP
#define LAMBDA_UTIL_HPP

#include "node.hpp"
#include "identifier.hpp"

namespace ast::util
{
    /**
     * Replaces recursively the identifier @param{id} in the tree with root @param{in}
     * by the node @param{by}
     *
     * Note: If id has conflits with abstraction,
     * this function renames the conflictive variables and identifiers appening "'" to the name end
     *
     * @return A copy of the tree with the identifiers replaced
     */
    Node::Reference replaceIdentifier(const Identifier::Reference &id, const Node::Reference &in, const Node::Reference &by);

    /**
     * Value nodes are:
     *  - Abstractions
     *
     * @return true if the if is a node value
     */
    bool isValueType(const Node &node);
}

#endif
