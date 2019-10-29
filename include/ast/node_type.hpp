#ifndef LAMBDA_NODE_TYPE_HPP
#define LAMBDA_NODE_TYPE_HPP

namespace ast::NodeType
{
    enum
    {
        Identifier,
        Variable,
        Abstraction,
        Application,

        NaturalConstant,
        BooleanConstant,
        Condition,
        LocalDefinition,
        Primitive,

        Unit,
        Sequence,
        Declaration,
    };
}

#endif
