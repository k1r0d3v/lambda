#include <ast/types/tuple_type.hpp>
#include <ast/types/type_kind.hpp>
#include <ast/types/dynamic_type.hpp>
#include <ast/exception.hpp>
#include <ast/node_kind.hpp>
#include <ast/natural_constant.hpp>


using namespace ast;

TupleType::TupleType(list<Type::Pointer> types)
    : OperableType(TypeKind::Tuple), mElements(std::move(types)) { }

bool TupleType::equals(const Type::Pointer &t) const
{
    auto tt = Type::cast<TupleType>(t);
    if (tt != nullptr)
    {
        if (tt->mElements.size() != mElements.size()) return false;

        for (size_t i = 0; i < mElements.size(); ++i)
            if (Type::distinct(mElements[i], tt->mElements[i]))
                return false;
        return true;
    }
    else if (t->kind() == TypeKind::Dyn) // TODO: Move this from here
        return true;

    return false;
}

Type::Pointer TupleType::typeof_dot(const Node::Pointer &b)
{
    if (b->kind() != NodeKind::NaturalConstant)
        throw AttributeException("\'" + b->toString() + "\' is not a natural");

    int index = Node::cast<NaturalConstant>(b)->value();
    if (index < 0 || index >= mElements.size())
        throw std::runtime_error(""); // TODO: Exception indexOutOfBounds

    return mElements[index];
}

Type::Pointer TupleType::typeof_index(int index)
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