#include <ast/node_kind.hpp>
#include <ast/conditional.hpp>
#include <ast/boolean_constant.hpp>
#include <ast/types/bool_type.hpp>
#include <ast/exception.hpp>
#include <ast/node_visitor.hpp>


using namespace ast;


Conditional::Conditional(Node::Pointer condition, Node::Pointer thenBranch, Node::Pointer elseBranch)
                : Node(NodeKind::Condition),
                  mCondition(std::move(condition)),
                  mThen(std::move(thenBranch)),
                  mElse(std::move(elseBranch)) { }

Node::Pointer Conditional::evaluate(Context &context) const
{
    auto evalResult = mCondition->evaluate(context);

    if (Node::cast<BooleanConstant>(evalResult)->value())
        return mThen->evaluate(context);
    else
        return mElse->evaluate(context);
}

Type::Pointer Conditional::typecheck(TypeContext &context)
{
    auto conditionType = mCondition->typecheck(context);

    if (!conditionType->isTypeOf(BoolType::INSTANCE))
        throw TypeException("Expected \'Bool\' not \'" + conditionType->toString() + "\'");

    auto thenType = mThen->typecheck(context);
    auto elseType = mElse->typecheck(context);

    if (!thenType->isTypeOf(elseType))
        throw TypeException("Incompatible operand types \'" + thenType->toString() + "\' and \'" + elseType->toString() + "\'");

    return thenType;
}

Node::Pointer Conditional::copy() const
{
    return Node::make<Conditional>(mCondition->copy(), mThen->copy(), mElse->copy());
}

string Conditional::toString() const
{
    auto os = std::ostringstream();
    os << "if " << mCondition->toString() << " then " << mThen->toString() << " else " << mElse->toString();
    return os.str();
}

Node::Pointer Conditional::transform(NodeVisitor *visitor)
{
    auto self = visitor->visitConditional(this);

    if (self != nullptr)
        return self;

    mCondition = Node::transform(mCondition, visitor);
    mThen = Node::transform(mThen, visitor);
    mElse = Node::transform(mElse, visitor);
    return nullptr;
}
