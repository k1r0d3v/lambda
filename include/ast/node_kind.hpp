#ifndef LAMBDA_NODE_KIND_HPP
#define LAMBDA_NODE_KIND_HPP

namespace ast::NodeKind
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

        // Not implemented
        OperatorDot,
        Fix
    };
}

#endif
