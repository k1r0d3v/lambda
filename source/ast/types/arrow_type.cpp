#include <ast/types/arrow_type.hpp>
#include <ast/types/type_kind.hpp>
#include <ast/types/top_type.hpp>

using namespace ast;

ArrowType::ArrowType(Type::Pointer left, Type::Pointer right)
    :  Type(TypeKind::Arrow), mLeft(std::move(left)), mRight(std::move(right)) { }

bool ArrowType::isTypeOf(const Type::Pointer &t) const
{
    auto tType = Type::cast<ArrowType>(t);
    if (tType != nullptr)
        return tType->mLeft->isTypeOf(mLeft) && tType->mRight->isTypeOf(mRight);

    return false;
}

bool ArrowType::isSubtypeOf(const Type::Pointer &t) const
{
    if (t->kind() == TypeKind::Top)
        return true;
    return false;
}

string ArrowType::toString() const
{
    auto os = std::ostringstream();

    if (mLeft->kind() == TypeKind::Arrow)
        os << "(" << mLeft->toString() << ")";
    else
        os << mLeft->toString();

    os << "->";

    if (mRight->kind() == TypeKind::Arrow)
        os << "(" << mRight->toString() << ")";
    else
        os << mRight->toString();

    return os.str();
}

void ArrowType::resolve(TypeContext &context)
{
    mLeft->resolve(context);
    mRight->resolve(context);
}
