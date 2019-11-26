#include <ast/types/undefined_type.hpp>
#include <ast/types/type_context.hpp>
#include <ast/exception.hpp>

using namespace ast;

bool UndefinedType::isTypeOf(const Type::Pointer &t) const
{
    if (mType != nullptr)
        return mType->isTypeOf(t);

    return false;
}

bool UndefinedType::isSubtypeOf(const Type::Pointer &t) const
{
    if (mType != nullptr)
        return mType->isSubtypeOf(t);

    return false;
}

void UndefinedType::resolve(TypeContext &context)
{
    if (mType == nullptr)
        mType = context.getTypeOf(this->name());

    if (mType == nullptr)
        throw TypeException("Undefined type \'" + this->toString() + "\'");
}

string UndefinedType::toString() const
{
    if (mType != nullptr)
        return mType->toString();
    else
        return ConstantType::toString();
}
