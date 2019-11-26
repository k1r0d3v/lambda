#include <ast/operator_dot.hpp>
#include <ast/node_kind.hpp>
#include <ast/operable_object.hpp>
#include <ast/types/operable_type.hpp>
#include <ast/exception.hpp>
#include <ast/node_visitor.hpp>

using namespace ast;

OperatorDot::OperatorDot(Node::Pointer source, Node::Pointer field)
    : Node(NodeKind::OperatorDot), mSource(std::move(source)), mField(std::move(field))
{ }

Node::Pointer OperatorDot::evaluate(Context &context) const
{
    auto operable = Node::cast<OperableObject>(mSource->evaluate(context));

    // For debug purposes
    if (operable == nullptr)
        throw AttributeException("\'" + this->toString() + "\' object has no attribute \'" + mField->toString() + "\'");

    return operable->operator_dot(mField, context);
}

Type::Pointer OperatorDot::typecheck(TypeContext &context)
{
    auto sourceType = Type::cast<OperableType>(mSource->typecheck(context));

    if (sourceType == nullptr)
        throw AttributeException("\'" + this->toString() + "\' object has no attribute \'" + mField->toString() + "\'");

    return sourceType->typeOfDot(mField);
}

Node::Pointer OperatorDot::copy() const
{
    return Node::make<OperatorDot>(mSource->copy(), mField->copy());
}

string OperatorDot::toString() const
{
    return mSource->toString() + "." + mField->toString();
}

Node::Pointer OperatorDot::transform(NodeVisitor *visitor)
{
    auto self = visitor->visitOperatorDot(this);

    if (self != nullptr)
        return self;

    mSource = Node::transform(mSource, visitor);
    // My field is not mean to be replaced, no?
    // mField = Node::transform(mField, visitor);
    return nullptr;
}
