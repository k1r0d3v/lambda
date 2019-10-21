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
    Node::Pointer replaceIdentifier(const Identifier::Pointer &id, const Node::Pointer &in, const Node::Pointer &by);
}

#endif
