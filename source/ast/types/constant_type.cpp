#include <ast/types/constant_type.hpp>
#include <ast/types/dynamic_type.hpp>

using namespace ast;

ConstantType::ConstantType(int kind, const string &name) : Type(kind), mName(name) { }

bool ConstantType::equals(const Type::Pointer &t) const
{
    auto ct = Type::cast<ConstantType>(t);
    if (ct != nullptr)
        return mName == ct->mName;
    else if (t->kind() == TypeKind::Dyn) // TODO: Move this from here
        return true;
    return false;
}

string ConstantType::toString() const
{
    return mName;
}