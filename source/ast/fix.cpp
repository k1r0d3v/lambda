#include <ast/fix.hpp>
#include <ast/node_kind.hpp>
#include <ast/types/arrow_type.hpp>
#include <ast/exception.hpp>
#include <ast/abstraction.hpp>
#include <ast/context.hpp>
#include <ast/node_visitor.hpp>


using namespace ast;


Fix::Fix(Node::Pointer term) : Node(NodeKind::Fix), mTerm(std::move(term)) { }

Node::Pointer Fix::evaluate(Context &context) const
{
    // TODO: Optimize this function

    auto abstraction = Node::cast<Abstraction>(mTerm->evaluate(context));

    // Push variable
    auto previous = context.setValue(abstraction->variable()->name(), Node::make<Fix>(abstraction->copy()));

    // Evaluate
    auto body = Node::transform(abstraction->body(), VariableReplacer(abstraction->variable(), context));

    // Pop variable
    context.setValue(abstraction->variable()->name(), previous);

    return body;
}

Type::Pointer Fix::typecheck(TypeContext &context)
{
    auto typeTerm = mTerm->typecheck(context);
    auto arrowType = Type::cast<ArrowType>(typeTerm);

    if (arrowType == nullptr)
        throw TypeException("\'" + typeTerm->toString() + "\' is not an abstraction");

    if (!arrowType->left()->isTypeOf(arrowType->right()))
        throw TypeException("\'" + arrowType->left()->toString() + "\' is not \'" + arrowType->right()->toString() + "\'");

    return arrowType->left();
}

Node::Pointer Fix::copy() const
{
    return Node::make<Fix>(mTerm->copy());
}

string Fix::toString() const
{
    return "fix (" + mTerm->toString() + ")";
}

Node::Pointer Fix::transform(NodeVisitor *visitor)
{
    return visitor->visitFix(this);
}
