#include <ast/types/type.hpp>
#include <ast/exception.hpp>

using namespace ast;

bool Type::equals(const Type::Pointer &a, const Type::Pointer &b)
{
    return a->equals(b);
}

bool Type::distinct(const Type::Pointer &a, const Type::Pointer &b)
{
    return !a->equals(b);
}

Type::Type(int kind) : mKind(kind) { }
