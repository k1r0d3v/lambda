#include <ast/list.hpp>
#include <ast/node_kind.hpp>
#include <ast/node_visitor.hpp>
#include <ast/types/list_type.hpp>
#include <ast/types/top_type.hpp>
#include <ast/exception.hpp>
#include <utility>

using namespace ast;

List::List(vector<Node::Pointer> elements) :
        Pattern(NodeKind::List), mConcatList(std::move(elements)) { }

List::List(Type::Pointer elementType)
    : Pattern(NodeKind::List), mElementType(std::move(elementType)) { }

List::List(Node::Pointer head, List::Pointer tail)
        :Pattern(NodeKind::List), mHead(std::move(head)), mTail(std::move(tail)) {}

static List::Pointer vectorToList(size_t index, const vector<Node::Pointer> &v, Context &context)
{
    assert(!v.empty());

    if (index == 0)
        return Node::cast<List>(v[index]->evaluate(context));

    auto tail = vectorToList(index - 1, v, context);

    if (tail->head() == nullptr) // If empty list
        return Node::make<List>(v[index]->evaluate(context), tail->tail());
    else
        return Node::make<List>(v[index]->evaluate(context), tail);
}

Node::Pointer List::evaluate(Context &context) const
{
    if (!mConcatList.empty())
        return vectorToList(mConcatList.size() - 1, mConcatList, context);

    if (mHead == nullptr)
        return Node::make<List>(mElementType);

    List::Pointer tail = nullptr;
    if (mTail != nullptr)
        tail = Node::cast<List>(mTail->evaluate(context));

    return Node::make<List>(mHead->evaluate(context), tail);
}


Type::Pointer List::typecheck(TypeContext &context)
{
    // If is a concat_list
    if (!mConcatList.empty())
    {
        auto last = mConcatList[0];
        auto lastType = last->typecheck(context);
        if (lastType->kind() != TypeKind::List)
            throw TypeException("Expected a last element list");

        mElementType = Type::cast<ListType>(lastType)->type();

        for (auto i = mConcatList.size() - 2; i > 0; i--)
        {
            auto t = mConcatList[i]->typecheck(context);
            if (!t->isTypeOf(mElementType))
                throw TypeException(
                        "List element type is \'" + t->toString() + "\' not \'" + mElementType->toString() + "\'");
        }

        return Type::make<ListType>(mElementType);
    }

    // Else lineal type check of elements
    if (mHead == nullptr) // Empty list
        return Type::make<ListType>(mElementType);

    if (mElementType == nullptr)
        mElementType = mHead->typecheck(context);

    auto head = mHead;
    auto tail = mTail;

    if (tail != nullptr)
    {
        do
        {
            head = tail->mHead;
            tail = tail->mTail;

            auto t = head->typecheck(context);
            if (!t->isTypeOf(mElementType))
                throw TypeException(
                        "List element type is \'" + t->toString() + "\' not \'" + mElementType->toString() + "\'");
        } while (tail != nullptr);
    }

    return Type::make<ListType>(mElementType);
}

Node::Pointer List::copy() const
{
    // TODO: Shallow or deep copy?
    auto tmp = Node::make<List>(mElementType);
    tmp->mHead = mHead;
    tmp->mTail = mTail;
    tmp->mConcatList = mConcatList;
    return tmp;
}

string List::toString() const
{
    string str;
    str += "[";

    if (!mConcatList.empty())
    {
        for (auto it = mConcatList.rbegin(); it != mConcatList.rend(); it++)
            str += (*it)->toString() + "::";

        if (*str.rbegin() == ':')
            return str.substr(0, str.size() - 2) + "]";
        return str + "]";
    }

    if(mHead == nullptr)
        return "[]";
    else
        str += mHead->toString();

    if (mTail != nullptr)
        str += mTail->toStringTail();
    else
        str += "]";

    return str;
}

string List::toStringTail() const{
    string str;
    if(mHead == nullptr)
        str += "]";
    else
        str += ", " + mHead->toString();

    if (mTail != nullptr)
        str += mTail->toStringTail();
    else
        str += "]";
    return str;
}

Node::Pointer List::transform(NodeVisitor *visitor)
{
    auto self = visitor->visitList(this);

    if (self != nullptr)
        return self;

    if (!mConcatList.empty())
    {
        for (auto &i : mConcatList)
            i = Node::transform(i, visitor);
    }
    else
    {
        if (mHead != nullptr)
            mHead = Node::transform(mHead, visitor);
        if (mTail != nullptr)
            mTail = Node::cast<List>(Node::transform(mTail, visitor));
    }

    return nullptr;
}

Pattern::MatchIdenfiers List::matchIdentifiers() const
{
    Pattern::MatchIdenfiers tmp;

    for (const auto &i : mConcatList)
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

Pattern::MatchResult List::match(const Node::Pointer &value, Context &context) const
{
    Pattern::MatchResult tmp = {};

    auto valueList = Node::cast<List>(value);

    if (valueList->mHead == nullptr)
        throw MatchException("Empty lists can not be matched");

    auto matchResult = Node::cast<Pattern>(mConcatList[1])->match(valueList->mHead, context);

    // Recollect matching results of head
    for (const auto &j : matchResult)
        tmp.push_back(j);

    auto tail = valueList->mTail != nullptr ? valueList->mTail : Node::make<List>(mElementType);

    matchResult = Node::cast<Pattern>(mConcatList[0])->match(tail, context);

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

    auto iPattern = Node::cast<Pattern>(mConcatList[1]);
    if (iPattern == nullptr)
        throw MatchException("\'" + mConcatList[1]->toString() + "\' is not a pattern");

    auto matchResult = iPattern->typecheckMatch(listType->type(), context);

    for (const auto &j : matchResult)
        tmp.push_back(j);

    iPattern = Node::cast<Pattern>(mConcatList[0]);
    if (iPattern == nullptr)
        throw MatchException("\'" + mConcatList[0]->toString() + "\' is not a pattern");

    matchResult = iPattern->typecheckMatch(listType, context);

    for (const auto &j : matchResult)
        tmp.push_back(j);
    return tmp;
}