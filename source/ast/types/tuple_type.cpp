#include <ast/types/tuple_type.hpp>
#include <ast/types/type_kind.hpp>
#include <ast/types/top_type.hpp>
#include <ast/exception.hpp>
#include <ast/node_kind.hpp>
#include <ast/natural_constant.hpp>


using namespace ast;

TupleType::TupleType(list<Type::Pointer> types)
    : OperableType(TypeKind::Tuple), mElements(std::move(types)) { }

bool TupleType::isTypeOf(const Type::Pointer &t) const
{
    auto tType = Type::cast<TupleType>(t);
    if (tType != nullptr)
    {
        if (tType->mElements.size() != mElements.size()) return false;

        for (size_t i = 0; i < mElements.size(); ++i)
            if (!tType->mElements[i]->isTypeOf(mElements[i]))
                return false;
        return true;
    }

    return false;
}

bool TupleType::isSubtypeOf(const Type::Pointer &t) const
{
    if (t->kind() == TypeKind::Top)
        return true;
    return false;
}

Type::Pointer TupleType::typeOfDot(const Node::Pointer &b)
{
    if (b->kind() != NodeKind::NaturalConstant)
        throw AttributeException("\'" + b->toString() + "\' is not a natural");

    int index = Node::cast<NaturalConstant>(b)->value();
    if (index < 0 || index >= mElements.size())
        throw std::runtime_error(""); // TODO: Exception indexOutOfBounds

    return mElements[index];
}

Type::Pointer TupleType::typeofIndex(int index)
{
    if (index < 0 || index >= mElements.size())
        throw std::runtime_error(""); // TODO: Exception indexOutOfBounds

    return mElements[index];
}

string TupleType::toString() const
{
    string str = "{";
    for (const auto &i : mElements)
        str += i->toString() + ", ";

    if (!mElements.empty())
    {
        str.pop_back();
        str.pop_back();
    }

    str += "}";
    return str;
}

void TupleType::resolve(TypeContext &context)
{
    for (auto &i : mElements)
        i->resolve(context);
}
