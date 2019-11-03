#ifndef LAMBDA_REGISTER_HPP
#define LAMBDA_REGISTER_HPP

#include "common.hpp"
#include "node_type.hpp"
#include "node.hpp"
#include "ast/types/register_type.hpp"

namespace ast
{
    class Register : public Pattern
    {
    public:
        using Pointer = Node::PointerType<Register>;

    public:
        explicit Register(std::map<string, Node::Pointer> elements)
                : Pattern(NodeType::Register), mElements(std::move(elements))
        { }

        explicit Register(const std::map<string, Pattern::Pointer>& elements)
                : Pattern(NodeType::Register)
        {
            for (const auto &i : elements)
                mElements.insert(i);
        }

        const std::map<string, Node::Pointer> &elements() const {
            return mElements;
        }

        Pattern::MatchIdenfiers matchIdentifiers() const override
        {
            Pattern::MatchIdenfiers tmp;
            for (const auto &i : mElements)
            {
                auto iPattern = Node::cast<Pattern>(i.second);

                if (iPattern == nullptr)
                    throw MatchException( "\'" + i.second->toString() + "\' is not a pattern");

                auto matchResult = iPattern->matchIdentifiers();
                for (const auto &j : matchResult)
                    tmp.push_back(j);
            }
            return tmp;
        }

        Pattern::MatchResult match(const Node::Pointer &value, Context &context) const override
        {
            Pattern::MatchResult tmp = {};

            auto valueRegister = Node::cast<Register>(value);
            for (const auto &i : mElements)
            {
                auto attr = valueRegister->mElements.find(i.first);
                auto matchResult = Node::cast<Pattern>(i.second)->match(attr->second, context);

                // TODO: Check duplicated ids
                for (const auto &j : matchResult)
                    tmp.push_back(j);
            }

            return tmp;
        }

        Pattern::TypecheckMatchResult typecheckMatch(const Type::Pointer &type, TypeContext &context) const override
        {
            Pattern::TypecheckMatchResult tmp = {};

            auto typeRegister = Type::cast<RegisterType>(type);

            if (typeRegister == nullptr)
                throw MatchException("\'" + type->toString() + "\' is not a register");

            for (const auto &i : mElements)
            {
                auto iPattern = Node::cast<Pattern>(i.second);

                if (iPattern == nullptr)
                    throw MatchException( "\'" + i.second->toString() + "\' is not a pattern");

                auto matchResult = iPattern->typecheckMatch(typeRegister->typeOfField(i.first), context);

                // TODO: Check duplicated ids
                for (const auto &j : matchResult)
                    tmp.push_back(j);
            }

            return tmp;
        }

        Node::Pointer evaluate(const Node::Pointer &self, Context &context) const override
        {
            std::map<string, Node::Pointer> elements;

            for (const auto &i : mElements)
                elements.insert({i.first, i.second->evaluate(i.second, context)});

            return Node::make<Register>(elements);
        }

        Node::Pointer operator_dot(const Node::Pointer &self, const string &field, Context &context) const override
        {
            auto value = mElements.find(field)->second;
            return value->evaluate(value, context);
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            std::map<string, Type::Pointer> types;

            for (const auto &i : mElements)
                types.insert({i.first, i.second->typecheck(context)});

            return Type::make<RegisterType>(types);
        }

        Node::Pointer copy() const override
        {
            std::map<string, Node::Pointer> elements;

            for (const auto &i : mElements)
                elements.insert({i.first, i.second->copy()});

            return Node::make<Register>(elements);
        }

        string toString() const override
        {
            string str = "{";
            for (const auto &i : mElements)
                str += i.first + ": " + i.second->toString() + ", ";

            if (!mElements.empty())
            {
                str.pop_back();
                str.pop_back();
            }

            str += "}";
            return str;
        }

    private:
        std::map<string, Node::Pointer> mElements;
    };
}

#endif //LAMBDA_REGISTER_HPP
