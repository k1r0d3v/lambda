#ifndef LAMBDA_NODE_TYPE_HPP
#define LAMBDA_NODE_TYPE_HPP

namespace ast::NodeType
{
    enum
    {
        LambdaOption,

        Identifier,
        Abstraction,
        Application,

        Natural,
        Boolean,
        Condition,
        LocalDefinition,
        NaturalPrimitive,

        Unit,
        Sequence,
    };
}

#endif
