#include <ast/list.hpp>
#include <ast/node_kind.hpp>
#include <ast/node_visitor.hpp>
#include <ast/types/list_type.hpp>
#include <ast/types/top_type.hpp>
#include <ast/exception.hpp>
#include <utility>

using namespace ast;

List::List(Type::Pointer elementType)
    : Pattern(NodeKind::List), mElementType(std::move(elementType)) { }

List::List(Node::Pointer head, Node::Pointer tail)
        :Pattern(NodeKind::List), mHead(std::move(head)), mTail(std::move(tail))
{ }

Node::Pointer List::evaluate(Context &context) const
{
    if (this->empty())
        return Node::make<List>(mElementType);

    List::Pointer tail = nullptr;
    if (mTail != nullptr)
        tail = Node::cast<List>(mTail->evaluate(context));

    // We must preserve element type for matching purposes
    auto l = Node::make<List>(mElementType);
    l->mHead = mHead->evaluate(context);
    l->mTail = tail;
    return l;
}

Type::Pointer List::typecheck(TypeContext &context)
{
    // Empty list
    if (this->empty())
        return Type::make<ListType>(mElementType);

    // List with one element
    if (mTail == nullptr)
    {
        mElementType = mHead->typecheck(context);
        return Type::make<ListType>(mElementType);
    }

    auto headType = mHead->typecheck(context);
    auto tailType = mTail->typecheck(context);

    if (tailType->kind() != TypeKind::List)
        throw TypeException("List tail must be a List");

    auto tailElementType = Type::cast<ListType>(tailType)->type();
    if (!headType->isTypeOf(tailElementType))
        throw TypeException(
                "List element type is \'" + tailElementType->toString() + "\' not \'" + headType->toString() + "\'");

    mElementType = headType;
    return Type::make<ListType>(mElementType);
}

Node::Pointer List::copy() const
{
    // TODO: Shallow or deep copy?
    auto tmp = Node::make<List>(mElementType);
    tmp->mHead = mHead;
    tmp->mTail = mTail;
    return tmp;
}

string List::toString() const
{
    // If is a pattern
    if (mTail != nullptr && Node::cast<List>(mTail) == nullptr)
        return mHead->toString() + "::" +  mTail->toString();

    if (this->empty())
        return mElementType->toString() + "[]";

    string str;
    str += "[";

    if(this->empty())
        return "[]";
    else
        str += mHead->toString();

    if (mTail != nullptr)
        str += std::reinterpret_pointer_cast<List>(mTail)->toStringTail();
    else
        str += "]";

    return str;
}

string List::toStringTail() const{
    string str;
    if(mHead == nullptr)
        return "]";
    else
        str += ", " + mHead->toString();

    if (mTail != nullptr)
        str += std::reinterpret_pointer_cast<List>(mTail)->toStringTail();
    else
        str += "]";
    return str;
}

Node::Pointer List::transform(NodeVisitor *visitor)
{
    auto self = visitor->visitList(this);

    if (self != nullptr)
        return self;

    if (mHead != nullptr)
        mHead = Node::transform(mHead, visitor);

    if (mTail != nullptr)
        mTail = Node::transform(mTail, visitor);

    return nullptr;
}

Pattern::MatchIdenfiers List::matchIdentifiers() const
{
    Pattern::MatchIdenfiers tmp;

    // TODO: Remove duplicated code
    if (mHead != nullptr)
    {
        // Head
        auto iPattern = Node::cast<Pattern>(mHead);

        if (iPattern == nullptr)
            throw MatchException( "\'" + mHead->toString() + "\' is not a pattern");

        auto matchResult = iPattern->matchIdentifiers();
        for (const auto &j : matchResult)
            tmp.push_back(j);

        // Tail
        iPattern = Node::cast<Pattern>(mTail);
        if (iPattern == nullptr)
            throw MatchException( "\'" + mTail->toString() + "\' is not a pattern");

        matchResult = iPattern->matchIdentifiers();
        for (const auto &j : matchResult)
            tmp.push_back(j);
    }

    return tmp;
}

Pattern::MatchResult List::match(const Node::Pointer &value, Context &context) const
{
    Pattern::MatchResult tmp = {};

    auto valueList = Node::cast<List>(value);

    if (valueList->empty())
        throw MatchException("Empty lists can not be matched");

    auto matchResult = Node::cast<Pattern>(mHead)->match(valueList->mHead, context);

    // Recollect matching results of head
    for (const auto &j : matchResult)
        tmp.push_back(j);

    // Check when only lefts the empty list and propagate the type
    auto tail = valueList->mTail != nullptr ? valueList->mTail : Node::make<List>(valueList->mElementType);

    matchResult = Node::cast<Pattern>(mTail)->match(tail, context);

    // Recollect matching results of tail
    for (const auto &j : matchResult)
        tmp.push_back(j);

    return tmp;
}

Pattern::TypecheckMatchResult List::typecheckMatch(const Type::Pointer &type, TypeContext &context) const
{
    Pattern::TypecheckMatchResult tmp = {};

    auto listType = Type::cast<ListType>(type);

    if (listType == nullptr)
        throw MatchException("\'" + type->toString() + "\' is not a list");

    auto iPattern = Node::cast<Pattern>(mHead);
    if (iPattern == nullptr)
        throw MatchException("\'" + mHead->toString() + "\' is not a pattern");

    auto matchResult = iPattern->typecheckMatch(listType->type(), context);

    for (const auto &j : matchResult)
        tmp.push_back(j);

    iPattern = Node::cast<Pattern>(mTail);
    if (iPattern == nullptr)
        throw MatchException("\'" + mTail->toString() + "\' is not a pattern");

    matchResult = iPattern->typecheckMatch(listType, context);

    for (const auto &j : matchResult)
        tmp.push_back(j);

    return tmp;
}