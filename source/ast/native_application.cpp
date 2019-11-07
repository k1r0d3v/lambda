#include <ast/node_kind.hpp>
#include <ast/native_application.hpp>
#include <ast/context.hpp>
#include <ast/exception.hpp>
#include <ast/node_visitor.hpp>


using namespace ast;

NativeApplication::NativeApplication(string name, NativeFunction::ArgumentsType arguments, NativeFunction fun)
    : Node(NodeKind::NativeApplication), 
                mName(std::move(name)),
                mArguments(std::move(arguments)),
                mFunction(std::move(fun)) { }

Node::Pointer NativeApplication::evaluate(Context &context) const
{
    list<NativeArgument> arguments;
    for (const auto &i : mArguments)
        arguments.push_back({i.value->evaluate(context), i.valueType});
    return mFunction.run(arguments);
}

Type::Pointer NativeApplication::typecheck(TypeContext &context) const
{
    for (const auto &i : mArguments)
    {
        auto type = i.value->typecheck(context);
        if (Type::distinct(type, i.valueType))
            throw TypeException("\'" + type->toString() + "\' is not a \'" + i.valueType->toString() + "\'");
    }

    return mFunction.resultType;
}

Node::Pointer NativeApplication::copy() const
{
    list<NativeArgument> arguments;
    for (const auto &i : mArguments)
        arguments.push_back({i.value->copy(), i.valueType});

    return Node::make<NativeApplication>(mName, arguments, mFunction);
}

string NativeApplication::toString() const
{
    auto os = std::ostringstream();
    os << "(" << mName << " ";
    for (const auto &i : mArguments)
        os << i.value->toString();
    os << ")";

    return os.str();
}

Node::Pointer NativeApplication::transform(NodeVisitor *visitor)
{
    auto self = visitor->visitNativeApplication(this);

    if (self != nullptr)
        return self;

    for (auto &i : mArguments)
        i.value = Node::transform(i.value, visitor);

    return nullptr;
}
