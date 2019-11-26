#include <ast/abstraction.hpp>
#include <ast/types/arrow_type.hpp>
#include <ast/node_visitor.hpp>
#include <ast/context.hpp>
#include <ast/exception.hpp>
#include <ast/types/undefined_type.hpp>

using namespace ast;

Abstraction::Abstraction(Variable::Pointer variable, Node::Pointer body)
            : Node(NodeKind::Abstraction),
              mVariable(std::move(variable)),
              mBody(std::move(body)) { }

Node::Pointer Abstraction::evaluate(Context &context) const
{
    if (!mResolved)
    {
        // Resolve id's
        auto abstraction = Node::cast<Abstraction>(Node::transform(this->copy(), IdentifierResolver(context)));
        abstraction->mResolved = true;
        return abstraction;
    }

    return this->copy();
}

Type::Pointer Abstraction::typecheck(TypeContext &context)
{
    // Push argument
    auto lastArgumentType = context.setTypeFor(mVariable->name(), mVariable->type());

    // Resolve aliases
    Type::Pointer varType = mVariable->type();
    varType->resolve(context);

    // Typecheck
    auto arrowType = Type::make<ArrowType>(varType, mBody->typecheck(context));

    // Pop argument
    context.setTypeFor(mVariable->name(), lastArgumentType);
    return arrowType;
}

Node::Pointer Abstraction::copy() const
{
    return Node::make<Abstraction>(Node::cast<Variable>(mVariable->copy()), mBody->copy());
}

string Abstraction::toString() const
{
    // Note: Replace will rename problematic entries
    auto os = std::ostringstream();
    os << "(\xce\xbb" << mVariable->toString() << " : "
       << mVariable->type()->toString() << ". "
       << mBody->toString() << ")";
    return os.str();
}

Node::Pointer Abstraction::transform(NodeVisitor *visitor)
{
    auto self = visitor->visitAbstraction(this);

    if (self != nullptr)
        return self;

    mVariable = Node::cast<Variable>(Node::transform(mVariable, visitor));
    mBody = Node::transform(mBody, visitor);

    return nullptr;
}
