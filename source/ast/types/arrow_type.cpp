#include <ast/types/arrow_type.hpp>
#include <ast/types/type_kind.hpp>
#include <ast/types/dynamic_type.hpp>

using namespace ast;

ArrowType::ArrowType(Type::Pointer left, Type::Pointer right)
    :  Type(TypeKind::Arrow), mLeft(std::move(left)), mRight(std::move(right)) { }

bool ArrowType::equals(const Type::Pointer &t) const
{
    auto at = Type::cast<ArrowType>(t);
    if (at != nullptr)
        return Type::equals(mLeft, at->mLeft) && Type::equals(mRight, at->mRight);
    else if (t->kind() == TypeKind::Dyn) // TODO: Move this from here
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