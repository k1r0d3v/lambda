#include <ast/sequence.hpp>
#include <ast/node_kind.hpp>
#include <ast/unit.hpp>
#include <ast/types/unit_type.hpp>
#include <ast/node_visitor.hpp>

using namespace ast;


Node::Pointer Sequence::join(const Node::Pointer &a, const Node::Pointer &b)
{
    if (a == nullptr)
        return b;

    if (b == nullptr)
        return a;

    list<Node::Pointer> elements;

    if (a != nullptr)
    {
        if (a->kind() == NodeKind::Sequence)
        {
            auto aElements = Node::cast<Sequence>(a)->elements();
            elements.insert(elements.end(), aElements.begin(), aElements.end());
        } else elements.push_back(a);
    }

    if (b != nullptr)
    {
        if (b->kind() == NodeKind::Sequence)
        {
            auto bElements = Node::cast<Sequence>(b)->elements();
            elements.insert(elements.end(), bElements.begin(), bElements.end());
        } else elements.push_back(b);
    }

    return Node::make<Sequence>(elements);
}

Sequence::Sequence(const list<Node::Pointer> &elements)
        : Node(NodeKind::Sequence), mElements(elements) { }

Node::Pointer Sequence::evaluate(Context &context) const
{
    auto r = Node::cast<Node>(Node::make<Unit>());

    for (const auto& i : mElements)
        r = i->evaluate(context);

    return r;
}

/*
Node::Pointer Sequence::resolve(const Node::Pointer &self, Context &context) const
{
    list<Node::Pointer> elements;
    for (const auto &i : mElements)
        elements.push_back(i->resolve(i, context));

    return Node::make<Sequence>(elements);
}
*/

Type::Pointer Sequence::typecheck(TypeContext &context) const
{
    Type::Pointer type = UnitType::INSTANCE;

    for (const auto& i : mElements)
        type = i->typecheck(context);

    return type;
}

Node::Pointer Sequence::copy() const
{
    list<Node::Pointer> elements;
    for (const auto &i : mElements)
        elements.push_back(i->copy());

    return Node::make<Sequence>(elements);
}

string Sequence::toString() const
{
    auto os = std::ostringstream();

    for (const auto& i : mElements)
        os << i->toString() << "; ";

    auto str = os.str();

    // Remove trailing ;
    if (!str.empty())
    {
        str.pop_back();
        str.pop_back();
    }

    return str;
}

Node::Pointer Sequence::transform(NodeVisitor *visitor)
{
    auto self = visitor->visitSequence(this);

    if (self != nullptr)
        return self;

    for (size_t i = 0; i < mElements.size(); i++)
        mElements[i] = Node::transform(mElements[i], visitor);

    return nullptr;
}
