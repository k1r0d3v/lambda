#ifndef LAMBDA_NATIVE_APPLICATION_HPP
#define LAMBDA_NATIVE_APPLICATION_HPP

#include <ast/common.hpp>
#include <ast/node.hpp>


namespace ast
{
    struct NativeArgument
    {
        Node::Pointer value;
        Type::Pointer valueType;
    };

    struct NativeFunction
    {
        using ArgumentsType = list<NativeArgument>;
        using FunctionType = std::function<Node::Pointer(const ArgumentsType&)>;

        FunctionType run;
        Type::Pointer resultType;
    };

    class NativeApplication : public Node
    {
    public:
        using Pointer = Node::PointerType<NativeApplication>;

    public:
        explicit NativeApplication(string name, NativeFunction::ArgumentsType arguments, NativeFunction fun);

        Node::Pointer evaluate(Context &context) const override;

        Type::Pointer typecheck(TypeContext &context) override;

        Node::Pointer transform(NodeVisitor *visitor) override;

        Node::Pointer copy() const override;

        string toString() const override;

    protected:
        string mName;
        list<NativeArgument> mArguments;
        NativeFunction mFunction;
    };
}

#endif //LAMBDA_NATIVE_APPLICATION_HPP
