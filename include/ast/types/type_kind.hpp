#ifndef LAMBDA_TYPE_KIND_HPP
#define LAMBDA_TYPE_KIND_HPP

namespace ast::TypeKind
{
    enum
    {
        Arrow,
        Bool,
        Nat,
        Str,
        Unit,
        Dyn,
        Tuple,
        Register,
        Composed,
    };
}

#endif //LAMBDA_TYPE_KIND_HPP
