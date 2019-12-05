#include <ast/types/constant_type.hpp>
#include <ast/types/top_type.hpp>

using namespace ast;

ConstantType::ConstantType(int kind, const string &name) : Type(kind), mName(name) { }

bool ConstantType::isTypeOf(const Type::Pointer &t) const
{
    auto ct = Type::cast<ConstantType>(t);
    if (ct != nullptr)
        return mName == ct->mName;
    return false;
}

bool ConstantType::isSubtypeOf(const Type::Pointer &t) const
{
    return t->kind() == TypeKind::Top;
}

string ConstantType::toString() const
{
    return mName;
}