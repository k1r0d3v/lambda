#ifndef LAMBDA_NATIVE_APPLICATION_HPP
#define LAMBDA_NATIVE_APPLICATION_HPP

#include <functional>

#include "common.hpp"
#include "node.hpp"
#include "node_type.hpp"
#include "types/type.hpp"
#include "exception.hpp"

namespace ast
{
    struct NativeArgument
    {
        Node::Pointer value;
        Type::Pointer valueType;
    };

    struct NativeFunction
    {
        std::function<Node::Pointer(const list<NativeArgument>&)> run;
        Type::Pointer resultType;
    };

    class NativeApplication : public Node
    {
    public:
        using Pointer = Node::PointerType<NativeApplication>;

    public:
        explicit NativeApplication(string name, list<NativeArgument> arguments, NativeFunction fun)
                : Node(NodeType::NativeApplication), mName(std::move(name)), mArguments(std::move(arguments)), mFunction(std::move(fun))
        { }

        Node::Pointer evaluate(const Node::Pointer &self, Context &context) const override
        {
            list<NativeArgument> arguments;
            for (const auto &i : mArguments)
                arguments.push_back({i.value->evaluate(i.value, context), i.valueType});
            return mFunction.run(arguments);
        }

        Node::Pointer resolve(const Node::Pointer &self, Context &context) const override
        {
            list<NativeArgument> arguments;
            for (const auto &i : mArguments)
                arguments.push_back({i.value->resolve(i.value, context), i.valueType});

            return Node::make<NativeApplication>(mName, arguments, mFunction);
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            for (const auto &i : mArguments)
            {
                auto type = i.value->typecheck(context);
                if (Type::distinct(type, i.valueType))
                    throw TypeException("\'" + type->toString() + "\' is not a \'" + i.valueType->toString() + "\'");
            }

            return mFunction.resultType;
        }

        Node::Pointer copy() const override
        {
            return Node::make<NativeApplication>(mName, mArguments, mFunction);
        }

        string toString() const override
        {
            auto os = std::ostringstream();
            os << "(" << mName << " ";
            for (const auto &i : mArguments)
                os << i.value->toString();
            os << ")";

            return os.str();
        }

    protected:
        string mName;
        list<NativeArgument> mArguments;
        NativeFunction mFunction;
    };
}

#endif //LAMBDA_NATIVE_APPLICATION_HPP
