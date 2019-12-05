#include <ast/list.hpp>
#include <ast/node_kind.hpp>
#include <ast/node_visitor.hpp>
#include <ast/types/list_type.hpp>
#include <ast/types/top_type.hpp>
#include <ast/exception.hpp>
#include <utility>

using namespace ast;

List::List(vector<Node::Pointer> elements) :
        Pattern(NodeKind::List), mTemporal(std::move(elements)) { }

List::List(Type::Pointer elementType)
    : Pattern(NodeKind::List), mElementType(std::move(elementType)) { }

List::List(Node::Pointer head, List::Pointer tail)
        :Pattern(NodeKind::List), mHead(std::move(head)), mTail(std::move(tail)) {}

Node::Pointer List::evaluate(Context &context) const
{
    if (mHead == nullptr)
        return Node::make<List>(mElementType);

    List::Pointer tail = nullptr;
    if (mTail != nullptr)
        tail = Node::cast<List>(mTail->evaluate(context));

    return Node::make<List>(mHead->evaluate(context), tail);
}

static List::Pointer vectorToList(size_t index, vector<Node::Pointer> &v)
{
    assert(!v.empty());

    if (index == 0)
        return Node::cast<List>(v[index]);

    auto tail = vectorToList(index - 1, v);
    if (tail->head() == nullptr) // If empty list
        return Node::make<List>(v[index], tail->tail());
    else
        return Node::make<List>(v[index], tail);
}

Type::Pointer List::typecheck(TypeContext &context)
{
    if (!mTemporal.empty())
    {
        auto last = mTemporal[0];
        if (last->typecheck(context)->kind() != TypeKind::List)
            throw TypeException("Last element must be a list");

        auto l = vectorToList(mTemporal.size() - 1, mTemporal);
        mHead = l->head();
        mTail = l->tail();
        mTemporal.clear();
    }

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
    if(mHead == nullptr){
        return Node::make<List>(vector<Node::Pointer>());
    }
    return Node::make<List>(mHead->copy(), Node::cast<List>(mTail->copy()));
}

string List::toString() const
{
    string str;
    str += "[";

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

    mHead = Node::transform(mHead, visitor);
    mTail = Node::cast<List>(Node::transform(mTail, visitor));
    return nullptr;
}

Pattern::MatchIdenfiers List::matchIdentifiers() const
{
    Pattern::MatchIdenfiers tmp;
    auto head = mHead;
    auto tail = mTail;

    while (head != nullptr)
    {
        auto iPattern = Node::cast<Pattern>(head);

        if (iPattern == nullptr)
            throw MatchException( "\'" + head->toString() + "\' is not a pattern");

        auto matchResult = iPattern->matchIdentifiers();
        for (const auto &j : matchResult)
            tmp.push_back(j);
        if(tail != nullptr)
        {
            head = tail->mHead;
            tail = tail->mTail;
        }
    }

    return tmp;
}

Pattern::MatchResult List::match(const Node::Pointer &value, Context &context) const
{
    Pattern::MatchResult tmp = {};
    auto valueList = Node::cast<List>(value);
    auto head = mHead;
    auto tail = mTail;
    Pattern::MatchResult matchResult;

    while (head != nullptr)
    {
        if (valueList == nullptr)
            throw MatchException("Unexpected pattern length");

        if(tail->mHead == nullptr){
            matchResult = Node::cast<Pattern>(head)->match(valueList, context);
        }else{
            matchResult = Node::cast<Pattern>(head)->match(valueList->mHead, context);
        }

        // Recollect matching results
        for (const auto &j : matchResult)
            tmp.push_back(j);

        if(tail != nullptr)
        {
            head = tail->mHead;
            tail = tail->mTail;
            valueList = valueList->mTail;
        }
    }

    return tmp;
}

Pattern::TypecheckMatchResult List::typecheckMatch(const Type::Pointer &type, TypeContext &context) const
{
    Pattern::TypecheckMatchResult tmp = {};

    auto typeList = Type::cast<ListType>(type);
    auto head = mHead;
    auto tail = mTail;

    if (typeList == nullptr)
        throw MatchException("\'" + type->toString() + "\' is not a list");

    while (head != nullptr)
    {
        auto iPattern = Node::cast<Pattern>(head);

        if (iPattern == nullptr)
            throw MatchException( "\'" + head->toString() + "\' is not a pattern");

        auto matchResult = iPattern->typecheckMatch(typeList->type(), context);

        // TODO: Check duplicated ids
        for (const auto &j : matchResult)
            tmp.push_back(j);

        if(tail != nullptr)
        {
            head = tail->mHead;
            tail = tail->mTail;
        }
    }

    return tmp;
}