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
        StringConstant,
        Condition,
        LocalDefinition,
        NativeApplication,

        Unit,
        Sequence,
        Declaration,
        Ascription,
        Tuple,
        Register,
        Dynamic,
        OperatorDot,
    };
}

#endif
