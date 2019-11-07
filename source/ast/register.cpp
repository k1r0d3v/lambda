#include <ast/register.hpp>
#include <ast/node_kind.hpp>
#include <ast/types/register_type.hpp>
#include <ast/exception.hpp>
#include <ast/node_visitor.hpp>


using namespace ast;


Register::Register(std::map<string, Node::Pointer> elements)
                : Pattern(NodeKind::Register), mElements(std::move(elements)) { }

Register::Register(const std::map<string, Pattern::Pointer>& elements) : Pattern(NodeKind::Register)
{
    for (const auto &i : elements)
        mElements.insert(i);
}

Pattern::MatchIdenfiers Register::matchIdentifiers() const
{
    Pattern::MatchIdenfiers tmp;
    for (const auto &i : mElements)
    {
        auto iPattern = Node::cast<Pattern>(i.second);

        if (iPattern == nullptr)
            throw MatchException( "\'" + i.second->toString() + "\' is not a pattern");

        auto matchResult = iPattern->matchIdentifiers();
        for (const auto &j : matchResult)
            tmp.push_back(j);
    }
    return tmp;
}

Pattern::MatchResult Register::match(const Node::Pointer &value, Context &context) const
{
    Pattern::MatchResult tmp = {};

    auto valueRegister = Node::cast<Register>(value);
    for (const auto &i : mElements)
    {
        auto attr = valueRegister->mElements.find(i.first);
        auto matchResult = Node::cast<Pattern>(i.second)->match(attr->second, context);

        // TODO: Check duplicated ids
        for (const auto &j : matchResult)
            tmp.push_back(j);
    }

    return tmp;
}

Pattern::TypecheckMatchResult Register::typecheckMatch(const Type::Pointer &type, TypeContext &context) const
{
    Pattern::TypecheckMatchResult tmp = {};

    auto typeRegister = Type::cast<RegisterType>(type);

    if (typeRegister == nullptr)
        throw MatchException("\'" + type->toString() + "\' is not a register");

    for (const auto &i : mElements)
    {
        auto iPattern = Node::cast<Pattern>(i.second);

        if (iPattern == nullptr)
            throw MatchException( "\'" + i.second->toString() + "\' is not a pattern");

        auto matchResult = iPattern->typecheckMatch(typeRegister->typeof_name(i.first), context);

        // TODO: Check duplicated ids
        for (const auto &j : matchResult)
            tmp.push_back(j);
    }

    return tmp;
}

Node::Pointer Register::evaluate(Context &context) const
{
    std::map<string, Node::Pointer> elements;

    for (const auto &i : mElements)
        elements.insert({i.first, i.second->evaluate(context)});

    return Node::make<Register>(elements);
}

Type::Pointer Register::typecheck(TypeContext &context) const
{
    std::map<string, Type::Pointer> types;

    for (const auto &i : mElements)
        types.insert({i.first, i.second->typecheck(context)});

    return Type::make<RegisterType>(types);
}

Node::Pointer Register::copy() const
{
    std::map<string, Node::Pointer> elements;

    for (const auto &i : mElements)
        elements.insert({i.first, i.second->copy()});

    return Node::make<Register>(elements);
}

string Register::toString() const
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

Node::Pointer Register::transform(NodeVisitor *visitor)
{
    auto self = visitor->visitRegister(this);

    if (self != nullptr)
        return self;

    for (auto &i : mElements)
        i.second = Node::transform(i.second, visitor);

    return nullptr;
}

Node::Pointer Register::operator_dot(const Node::Pointer &b, Context &context) const
{
    // TODO: Do checks? Or leave it to the typecheck?
    return mElements.find(Node::cast<Identifier>(b)->name())->second;
}
