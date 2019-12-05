#ifndef LAMBDA_TYPE_KIND_HPP
#define LAMBDA_TYPE_KIND_HPP

namespace ast::TypeKind
{
    enum
    {
        Arrow,
        Bool,
        Nat,
        Float,
        Str,
        Unit,
        Top,
        Tuple,
        List,
        Register,
        Undefined,
    };
}

#endif //LAMBDA_TYPE_KIND_HPP
