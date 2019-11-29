#ifndef LAMBDA_NODE_VISITOR_HPP
#define LAMBDA_NODE_VISITOR_HPP

#include <ast/node.hpp>
#include <ast/ascription.hpp>
#include <ast/conditional.hpp>
#include <ast/local_definition.hpp>
#include <ast/abstraction.hpp>
#include <ast/application.hpp>
#include <ast/identifier.hpp>
#include <ast/variable.hpp>
#include <ast/tuple.hpp>
#include <ast/register.hpp>
#include <ast/sequence.hpp>
#include <ast/declaration.hpp>
#include <ast/natural_constant.hpp>
#include <ast/float_constant.hpp>
#include <ast/boolean_constant.hpp>
#include <ast/string_constant.hpp>
#include <ast/native_application.hpp>
#include <ast/unit.hpp>
#include <ast/operator_dot.hpp>
#include <ast/fix.hpp>
#include <ast/alias.hpp>


namespace ast
{
    class NodeVisitor
    {
    public:
        virtual Node::Pointer visitAscription(Ascription *node)
        {
            return nullptr;
        }

        virtual Node::Pointer visitConditional(Conditional *node)
        {
            return nullptr;
        }

        virtual Node::Pointer visitLocalDefinition(LocalDefinition *node)
        {
            return nullptr;
        }

        virtual Node::Pointer visitAbstraction(Abstraction *node)
        {
            return nullptr;
        }

        virtual Node::Pointer visitApplication(Application *node)
        {
            return nullptr;
        }

        virtual Node::Pointer visitIdentifier(Identifier *node)
        {
            return nullptr;
        }

        virtual Node::Pointer visitVariable(Variable *node)
        {
            return nullptr;
        }

        virtual Node::Pointer visitTuple(Tuple *node)
        {
            return nullptr;
        }

        virtual Node::Pointer visitRegister(Register *node)
        {
            return nullptr;
        }

        virtual Node::Pointer visitSequence(Sequence *node)
        {
            return nullptr;
        }

        virtual Node::Pointer visitDeclaration(Declaration *node)
        {
            return nullptr;
        }

        virtual Node::Pointer visitNatural(NaturalConstant *node)
        {
            return nullptr;
        }

        virtual Node::Pointer visitFloat(FloatConstant *node)
        {
            return nullptr;
        }

        virtual Node::Pointer visitString(StringConstant *node)
        {
            return nullptr;
        }

        virtual Node::Pointer visitBoolean(BooleanConstant *node)
        {
            return nullptr;
        }

        virtual Node::Pointer visitNativeApplication(NativeApplication *node)
        {
            return nullptr;
        }

        virtual Node::Pointer visitUnit(Unit *node)
        {
            return nullptr;
        }

        virtual Node::Pointer visitFix(Fix *node)
        {
            return nullptr;
        }

        virtual Node::Pointer visitOperatorDot(OperatorDot *node)
        {
            return nullptr;
        }

        virtual Node::Pointer visitAlias(Alias *node)
        {
            return nullptr;
        }
    };

    class IdentifierResolver : public NodeVisitor
    {
    private:
        Context &mContext;

    public:
        explicit IdentifierResolver(Context &ctx);

        Node::Pointer visitIdentifier(Identifier *node) override;

        Node::Pointer visitAbstraction(Abstraction *node) override;

        Node::Pointer visitLocalDefinition(LocalDefinition *node) override;
    };

    class VariableReplacer : public NodeVisitor
    {
    private:
        const Variable::Pointer &mArg;
        Context &mContext;

    public:
        explicit VariableReplacer(const Variable::Pointer &arg, Context &ctx);

        Node::Pointer visitVariable(Variable *node) override;

        Node::Pointer visitAbstraction(Abstraction *node) override;
    };
}

#endif //LAMBDA_NODE_VISITOR_HPP
