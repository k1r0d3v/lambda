#include <ast/node_kind.hpp>
#include <ast/float_constant.hpp>
#include <ast/types/float_type.hpp>
#include <ast/node_visitor.hpp>


using namespace ast;

FloatConstant::FloatConstant(FloatValueType value)
        : Node(NodeKind::FloatConstant), mValue(value) { }


Node::Pointer FloatConstant::evaluate(Context &context) const
{
    return this->copy();
}

Type::Pointer FloatConstant::typecheck(TypeContext &context)
{
    return FloatType::INSTANCE;
}

Node::Pointer FloatConstant::copy() const
{
    return Node::make<FloatConstant>(mValue);
}

string FloatConstant::toString() const
{
    return to_string(mValue);
}

Node::Pointer FloatConstant::transform(NodeVisitor *visitor)
{
    return visitor->visitFloat(this);
}
