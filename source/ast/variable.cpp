#include <ast/variable.hpp>
#include <ast/context.hpp>
#include <ast/exception.hpp>
#include <ast/node_visitor.hpp>


using namespace ast;

Variable::Variable(string name, Type::Pointer type)
        : Node(NodeKind::Variable), mName(std::move(name)), mType(std::move(type))
{
    assert(mType != nullptr);
}

// Only called in the body of abstractions
Node::Pointer Variable::evaluate(Context &context) const
{
    // Should not happen if there is a typecheck before, this check is only for debug purposes
    throw NameException("Variable \'" + mName + "\' hasn't been replaced");
}

Type::Pointer Variable::typecheck(TypeContext &context) const
{
    return mType;
}

Node::Pointer Variable::copy() const
{
    return Node::make<Variable>(mName, mType);
}

string Variable::toString() const
{
    return mName;
}

Node::Pointer Variable::transform(NodeVisitor *visitor)
{
    return visitor->visitVariable(this);
}
