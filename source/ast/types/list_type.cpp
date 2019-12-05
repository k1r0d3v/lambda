#include <ast/types/list_type.hpp>
#include <ast/node_kind.hpp>
#include <ast/exception.hpp>
#include <ast/natural_constant.hpp>

using namespace ast;

ListType::ListType(Type::Pointer type)
        : ConstantType(TypeKind::List, "List"), mType(std::move(type)) { }

bool ListType::isTypeOf(const Type::Pointer &t) const
{
    if (t->kind() == TypeKind::List)
        return Type::cast<ListType>(t)->mType->isTypeOf(mType);
    return false;
}

bool ListType::isSubtypeOf(const Type::Pointer &t) const
{
    return t->kind() == TypeKind::Top;
}

string ListType::toString() const
{
    return "[" + mType->toString() + "]";
}

void ListType::resolve(TypeContext &context)
{
    mType->resolve(context);
}

