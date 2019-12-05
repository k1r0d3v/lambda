#include <ast/types/register_type.hpp>
#include <ast/types/type_kind.hpp>
#include <ast/types/top_type.hpp>
#include <ast/exception.hpp>
#include <ast/node_kind.hpp>
#include <ast/identifier.hpp>

using namespace ast;


RegisterType::RegisterType(std::map<string, Type::Pointer> types)
    : OperableType(TypeKind::Register), mElements(std::move(types)) { }

bool RegisterType::isTypeOf(const Type::Pointer &t) const
{
    auto tType = Type::cast<RegisterType>(t);
    if (tType != nullptr)
    {
        if (tType->mElements.size() != mElements.size()) return false;

        for (const auto &i : mElements)
        {
            // Search the entries
            auto otherValue = tType->mElements.find(i.first);
            auto myValue = mElements.find(i.first);

            // Entry not found
            if (otherValue == tType->mElements.end()) return false;

            // Distinct type of entries
            if (!otherValue->second->isTypeOf(myValue->second))
                return false;
        }
        return true;
    }
    return false;
}

bool RegisterType::isSubtypeOf(const Type::Pointer &t) const
{
    return t->kind() == TypeKind::Top;
}

Type::Pointer RegisterType::typeOfDot(const Node::Pointer &b)
{
    if (b->kind() != NodeKind::Identifier)
        throw AttributeException("\'" + this->toString() + "\' object has no attribute \'" + b->toString() + "\'");

    auto name = Node::cast<Identifier>(b)->name();
    auto value = mElements.find(name);
    if (value == mElements.end())
        throw AttributeException("\'" + this->toString() + "\' object has no attribute \'" + name + "\'");
    return value->second;
}

Type::Pointer RegisterType::typeOfName(const string &name)
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

void RegisterType::resolve(TypeContext &context)
{
    for (auto &i : mElements)
        i.second->resolve(context);
}
