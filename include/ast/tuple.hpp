#ifndef LAMBDA_TUPLE_HPP
#define LAMBDA_TUPLE_HPP

#include "common.hpp"
#include "node_type.hpp"
#include "node.hpp"
#include "ast/types/tuple_type.hpp"

namespace ast
{
    class Tuple : public Pattern
    {
    public:
        using Pointer = Node::PointerType<Tuple>;

    public:
        explicit Tuple(list<Node::Pointer> elements)
            : Pattern(NodeType::Tuple), mElements(std::move(elements)) { }

        const list<Node::Pointer> &elements() const {
            return mElements;
        }

        Pattern::MatchIdenfiers matchIdentifiers() const override
        {
            Pattern::MatchIdenfiers tmp;
            for (const auto &i : mElements)
            {
                auto iPattern = Node::cast<Pattern>(i);

                if (iPattern == nullptr)
                    throw MatchException( "\'" + i->toString() + "\' is not a pattern");

                auto matchResult = iPattern->matchIdentifiers();
                for (const auto &j : matchResult)
                    tmp.push_back(j);
            }
            return tmp;
        }

        Pattern::MatchResult match(const Node::Pointer &value, Context &context) const override
        {
            Pattern::MatchResult tmp = {};

            auto valueTuple = Node::cast<Tuple>(value);
            for (size_t i = 0; i < mElements.size(); ++i)
            {
                auto matchResult = Node::cast<Pattern>(mElements[i])->match(valueTuple->mElements[i], context);

                // Recollect matching results
                for (const auto &j : matchResult)
                    tmp.push_back(j);
            }

            return tmp;
        }

        Pattern::TypecheckMatchResult typecheckMatch(const Type::Pointer &type, TypeContext &context) const override
        {
            Pattern::TypecheckMatchResult tmp = {};

            auto typeTuple = Type::cast<TupleType>(type);

            if (typeTuple == nullptr)
                throw MatchException("\'" + type->toString() + "\' is not a tuple");

            if (typeTuple->elements().size() != mElements.size())
                throw MatchException("Pattern size \'" + to_string(mElements.size()) + "\' expected \'" + to_string(typeTuple->elements().size()) + "\'");

            for (size_t i = 0; i < mElements.size(); ++i)
            {
                auto iPattern = Node::cast<Pattern>(mElements[i]);
                if (iPattern == nullptr)
                    throw MatchException( "\'" + mElements[i]->toString() + "\' is not a pattern");

                auto iType = typeTuple->typeOfField(to_string(i));
                auto matchResult = iPattern->typecheckMatch(iType, context);

                // TODO: Check duplicated ids
                for (const auto &j : matchResult)
                    tmp.push_back(j);
            }

            return tmp;
        }

        Node::Pointer evaluate(const Node::Pointer &self, Context &context) const override
        {
            list<Node::Pointer> elements;

            for (const auto &i : mElements)
                elements.push_back(i->evaluate(i, context));

            return Node::make<Tuple>(elements);
        }

        Node::Pointer operator_dot(const Node::Pointer &self, const string &field, Context &context) const override
        {
            auto index = std::stoi(field);
            return mElements[index]->evaluate(mElements[index], context);
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            list<Type::Pointer> types;

            for (const auto &i : mElements)
                types.push_back(i->typecheck(context));

            return Type::make<TupleType>(types);
        }

        Node::Pointer copy() const override
        {
            list<Node::Pointer> elements;

            for (const auto &i : mElements)
                elements.push_back(i->copy());

            return Node::make<Tuple>(elements);
        }

        string toString() const override
        {
            string str = "{ ";
            for (const auto &i : mElements)
                str += i->toString() + ", ";

            if (!mElements.empty())
            {
                str.pop_back();
                str.pop_back();
            }

            str += " }";
            return str;
        }

    private:
        list<Node::Pointer> mElements;
    };
}

#endif //LAMBDA_TUPLE_HPP
