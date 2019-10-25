#ifndef LAMBDA_NODE_TYPE_HPP
#define LAMBDA_NODE_TYPE_HPP

namespace ast::NodeType
{
    enum
    {
        Identifier,
        Abstraction,
        Application,

        Natural,
        Boolean,
        Condition,
        LocalDefinition,
        Primitive,

        Unit,
        Sequence,
    };
}

#endif
