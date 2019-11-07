#include <ast/types/register_type.hpp>
#include <ast/types/type_kind.hpp>
#include <ast/types/dynamic_type.hpp>
#include <ast/exception.hpp>
#include <ast/node_kind.hpp>
#include <ast/identifier.hpp>

using namespace ast;


RegisterType::RegisterType(std::map<string, Type::Pointer> types)
    : OperableType(TypeKind::Register), mElements(std::move(types)) { }

bool RegisterType::equals(const Type::Pointer &t) const
{
    auto tt = Type::cast<RegisterType>(t);
    if (tt != nullptr)
    {
        if (tt->mElements.size() != mElements.size()) return false;

        for (const auto &i : mElements)
        {
            // Search the entries
            auto otherValue = tt->mElements.find(i.first);
            auto myValue = mElements.find(i.first);

            // Entry not found
            if (otherValue == tt->mElements.end()) return false;

            // Distinct type of entries
            if (Type::distinct(myValue->second, otherValue->second))
                return false;
        }
        return true;
    }
    else if (t->kind() == TypeKind::Dyn) // TODO: Move this from here
        return true;

    return false;
}

Type::Pointer RegisterType::typeof_dot(const Node::Pointer &b)
{
    if (b->kind() != NodeKind::Identifier)
        throw AttributeException("\'" + this->toString() + "\' object has no attribute \'" + b->toString() + "\'");

    auto name = Node::cast<Identifier>(b)->name();
    auto value = mElements.find(name);
    if (value == mElements.end())
        throw AttributeException("\'" + this->toString() + "\' object has no attribute \'" + name + "\'");
    return value->second;
}

Type::Pointer RegisterType::typeof_name(const string &name)
{
    auto value = mElements.find(name);
    if (value == mElements.end())
        throw AttributeException("\'" + this->toString() + "\' object has no attribute \'" + name + "\'");
    return value->second;
}

string RegisterType::toString() const
{
    string str = "{";
    for (const auto &i : mElements)
        str += i.first + ": " + i.second->toString() + ", ";

    if (!mElements.empty())
    {
        str.pop_back();
        str.pop_back();
    }

    str += "}";
    return str;
}