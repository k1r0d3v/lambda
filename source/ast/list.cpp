#include <ast/list.hpp>
#include <ast/node_kind.hpp>
#include <ast/node_visitor.hpp>
#include <ast/types/list_type.hpp>
#include <ast/types/top_type.hpp>
#include <ast/exception.hpp>
#include <utility>

using namespace ast;
/*
List::List(const list<Node::Pointer> &elements) : Pattern(NodeKind::List)
{
    if (!elements.empty())
    {
        mHead = *elements.begin();
        auto foo = list<Node::Pointer>{elements.begin() + 1, elements.end()};
        mTail = Node::make<List>(foo);
    }
}*/
List::List(list<Node::Pointer> elements) :
        Pattern(NodeKind::List), mTemporal(std::move(elements)) {}

List::List(Node::Pointer head, List::Pointer tail)
        :Pattern(NodeKind::List), mHead(std::move(head)), mTail(std::move(tail)) {}

Node::Pointer List::evaluate(Context &context) const
{
    if (mHead == nullptr)
        return Node::make<List>(list<Node::Pointer>());
    auto headEval = Node::cast<List>(mHead->evaluate(context));
    /*if (headEval != nullptr && headEval->mHead == nullptr && mTail->mHead == nullptr )
    {
        return Node::make<List>(list<Node::Pointer>());
    } else if (headEval != nullptr && headEval->mHead == nullptr && mTail->mHead != nullptr){
        // TODO Excepcion La estoy liando porque no podria declarar listas de listas
        throw MatchException("Holaaa");
    }*/
    return Node::make<List>(mHead->evaluate(context), Node::cast<List>(mTail->evaluate(context)));
}

Type::Pointer List::typecheck(TypeContext &context)
{
    // In this code it is expected that the list is upside down and has more than 1 element
    if (!mTemporal.empty())
    {
        auto typeList = mTemporal[0]->typecheck(context);
        if(typeList->kind() != TypeKind::List){
            throw TypeException("The last element concat is not a List");
        }
        List::Pointer aux = Node::make<List>(mTemporal[0], nullptr);
        for(size_t i=1; i < mTemporal.size() - 1; i++){
            aux = Node::make<List>(mTemporal[i], aux);
        }
        mHead = mTemporal[mTemporal.size() - 1];
        mTail = aux;
        mTemporal.clear();
    }

    if (mHead == nullptr)
        return Type::make<ListType>(TopType::INSTANCE);

    auto type = mHead->typecheck(context);

    auto head = mTail->mHead;
    auto tail = mTail->mTail;
    while (head != nullptr)
    {
        if(!type->isTypeOf(head->typecheck(context))){
            throw TypeException("Expected type : " + type->toString() + "not the type: "+ head->typecheck(context)->toString());
        }
        if(tail != nullptr)
        {
            head = tail->mHead;
            tail = tail->mTail;
        }
    }
    return Type::make<ListType>(type);
}

Node::Pointer List::copy() const
{
    if(mHead == nullptr){
        return Node::make<List>(list<Node::Pointer>());
    }
    return Node::make<List>(mHead->copy(), Node::cast<List>(mTail->copy()));
}

string List::toString() const
{
    string str;
    str += "[";

    if(mHead == nullptr)
    {
        str += "]";
    }else{
        str +=  mHead->toString() + mTail->toStringTail();
    }
    return str;
}

string List::toStringTail() const{
    string str;
    if(mHead == nullptr)
    {
        str += "]";
    }else{
        str += "; " + mHead->toString() + mTail->toStringTail();
    }
    return str;
}

Node::Pointer List::transform(NodeVisitor *visitor)
{
    auto self = visitor->visitList(this);

    if (self != nullptr)
        return self;

    if(mHead != nullptr)
    {
        auto h = mHead->transform(visitor);
        if(h != nullptr)
        {
            h = mHead;
            if (mTail != nullptr)
                mTail = Node::cast<List>(mTail->transform(visitor));
        }
    }
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
