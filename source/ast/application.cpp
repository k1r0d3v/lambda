#include <ast/node_kind.hpp>
#include <ast/abstraction.hpp>
#include <ast/application.hpp>
#include <ast/types/arrow_type.hpp>
#include <ast/exception.hpp>
#include <ast/node_visitor.hpp>
#include <ast/context.hpp>


using namespace ast;


Application::Application(Node::Pointer left, Node::Pointer right)
    : Node(NodeKind::Application),
    mLeft(std::move(left)),
    mRight(std::move(right))
{
    // Curry style application, reorder the tree
    if (mRight->kind() == NodeKind::Application)
    {
        auto myRight = Node::cast<Application>(mRight);
        mLeft = Node::make<Application>(mLeft, myRight->left());
        mRight = myRight->right();
    }
}

Node::Pointer Application::evaluate(Context &context) const
{
    // Creates a pointer to this with no deleter
    Node::Pointer t = this->copy();

    do
    {
        Application::Pointer application = Node::cast<Application>(t);

        // Call by value resolves the right most term first
        Node::Pointer leftTerm, rightTerm;
        leftTerm = application->left();
        rightTerm = application->right();

        // Evaluates left side if not an abstraction
        // We must resolve the abstraction calling evaluate in her
        leftTerm = leftTerm->evaluate(context);

        auto abstraction = Node::cast<Abstraction>(leftTerm);

        // Push arg
        auto previous = context.setValue(abstraction->variable()->name(), rightTerm->evaluate(context));

        // Evaluate
        t = Node::transform(abstraction->body(), VariableReplacer(abstraction->variable(), context));

        // Pop arg
        context.setValue(abstraction->variable()->name(), previous);

        // Try to reuse the stack to avoid stack overflows
    } while (t->kind() == NodeKind::Application);

    return t->evaluate(context);
}

Type::Pointer Application::typecheck(TypeContext &context) const
{
    auto leftType = Type::cast<ArrowType>(mLeft->typecheck(context));

    // TODO: Change exception messages
    if (leftType == nullptr)
        throw TypeException("Expected an abstraction");

    Type::Pointer rightType = mRight->typecheck(context);

    // TODO: Change exception messages
    if (Type::distinct(leftType->left(), rightType))
        throw TypeException("Incompatible types");

    return leftType->right();
}

Node::Pointer Application::copy() const
{
    return Node::make<Application>(mLeft->copy(), mRight->copy());
}

string Application::toString() const
{
    auto os = std::ostringstream();
    os << mLeft->toString() << " " << mRight->toString();
    return os.str();
}

Node::Pointer Application::transform(NodeVisitor *visitor)
{
    auto self = visitor->visitApplication(this);

    if (self != nullptr)
        return self;

    mLeft = Node::transform(mLeft, visitor);
    mRight = Node::transform(mRight, visitor);
    return nullptr;
}
