#include <ast/list_primitives.hpp>
#include <ast/boolean_constant.hpp>
#include <ast/list.hpp>
#include <ast/types/bool_type.hpp>
#include <ast/types/list_type.hpp>
#include <ast/types/top_type.hpp>
#include <ast/exception.hpp>

using namespace ast;

static Node::Pointer isempty(const NativeFunction::ArgumentsType &args)
{
    if (args[0].value->kind() != NodeKind::List)
        throw TypeException("isempty only works with lists");
    return Node::make<BooleanConstant>(Node::cast<List>(args[0].value)->empty());
}

IsEmpty::IsEmpty(Node::Pointer argument)
        : NativeApplication("isempty",
                            {{std::move(argument), TopType::INSTANCE}},
                            {isempty, BoolType::INSTANCE}) { }
