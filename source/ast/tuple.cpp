#include <ast/tuple.hpp>
#include <ast/node_kind.hpp>
#include <ast/types/tuple_type.hpp>
#include <ast/exception.hpp>
#include <ast/node_visitor.hpp>

using namespace ast;

Tuple::Tuple(list<Node::Pointer> elements)
                : Pattern(NodeKind::Tuple), mElements(std::move(elements)) { }

Pattern::MatchIdenfiers Tuple::matchIdentifiers() const
{
    Pattern::MatchIdenfiers tmp;
    for (const auto &i : mElements)
    {
        auto iPattern = Node::cast<Pattern>(i);

        if (iPattern == nullptr)
            throw MatchException( "\'" + i->toString() + "\' is not a pattern");

        auto matchResult = iPattern->matchIdentifiers();
        for (const auto &j : matchResult)
            tmp.push_back(j);
    }
    return tmp;
}

Pattern::MatchResult Tuple::match(const Node::Pointer &value, Context &context) const
{
    Pattern::MatchResult tmp = {};

    auto valueTuple = Node::cast<Tuple>(value);
    for (size_t i = 0; i < mElements.size(); ++i)
    {
        auto matchResult = Node::cast<Pattern>(mElements[i])->match(valueTuple->mElements[i], context);

        // Recollect matching results
        for (const auto &j : matchResult)
            tmp.push_back(j);
    }

    return tmp;
}

Pattern::TypecheckMatchResult Tuple::typecheckMatch(const Type::Pointer &type, TypeContext &context) const
{
    Pattern::TypecheckMatchResult tmp = {};

    auto typeTuple = Type::cast<TupleType>(type);

    if (typeTuple == nullptr)
        throw MatchException("\'" + type->toString() + "\' is not a tuple");

    if (typeTuple->elements().size() != mElements.size())
        throw MatchException("Pattern size \'" + to_string(mElements.size()) + "\' expected \'" + to_string(typeTuple->elements().size()) + "\'");

    for (size_t i = 0; i < mElements.size(); ++i)
    {
        auto iPattern = Node::cast<Pattern>(mElements[i]);
        if (iPattern == nullptr)
            throw MatchException( "\'" + mElements[i]->toString() + "\' is not a pattern");

        auto iType = typeTuple->typeofIndex(i);
        auto matchResult = iPattern->typecheckMatch(iType, context);

        // TODO: Check duplicated ids
        for (const auto &j : matchResult)
            tmp.push_back(j);
    }

    return tmp;
}

Node::Pointer Tuple::evaluate(Context &context) const
{
    list<Node::Pointer> elements;

    for (const auto &i : mElements)
        elements.push_back(i->evaluate(context));

    return Node::make<Tuple>(elements);
}

Type::Pointer Tuple::typecheck(TypeContext &context)
{
    list<Type::Pointer> types;

    for (const auto &i : mElements)
        types.push_back(i->typecheck(context));

    return Type::make<TupleType>(types);
}

Node::Pointer Tuple::copy() const
{
    list<Node::Pointer> elements;
    for (const auto &i : mElements)
        elements.push_back(i->copy());

    return Node::make<Tuple>(elements);
}

string Tuple::toString() const
{
    string str = "{ ";
    for (const auto &i : mElements)
        str += i->toString() + ", ";

    if (!mElements.empty())
    {
        str.pop_back();
        str.pop_back();
    }

    str += " }";
    return str;
}

Node::Pointer Tuple::transform(NodeVisitor *visitor)
{
    auto self = visitor->visitTuple(this);

    if (self != nullptr)
        return self;

    for (size_t i = 0; i < mElements.size(); i++)
        mElements[i] = Node::transform(mElements[i], visitor);

    return nullptr;
}

Node::Pointer Tuple::operatorDot(const Node::Pointer &b, Context &context) const
{
    // TODO: Do checks? Or leave it to the typecheck?
    return mElements[Node::cast<NaturalConstant>(b)->value()];
}
