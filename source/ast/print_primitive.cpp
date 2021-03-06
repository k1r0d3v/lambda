#include <ast/print_primitive.hpp>
#include <ast/node_kind.hpp>
#include <ast/string_constant.hpp>
#include <ast/natural_constant.hpp>
#include <ast/float_constant.hpp>
#include <ast/unit.hpp>
#include <ast/types/unit_type.hpp>
#include <ast/types/top_type.hpp>


using namespace ast;

static Node::Pointer print(const NativeFunction::ArgumentsType &args)
{
    auto arg = args[0].value;

    if (arg->kind() == NodeKind::StringConstant)
    {
        auto str = Node::cast<StringConstant>(arg);
        std::cout << str->value();
    }
    else if (arg->kind() == NodeKind::NaturalConstant)
    {
        auto nat = Node::cast<NaturalConstant>(arg);
        std::cout << nat->value();
    }
    else if (arg->kind() == NodeKind::FloatConstant)
    {
        auto flo = Node::cast<FloatConstant>(arg);
        std::cout << flo->value();
    }
    else std::cout << arg->toString();

    return Node::make<Unit>();
}

Print::Print(Node::Pointer argument)
    : NativeApplication("print",
                        {{std::move(argument), TopType::INSTANCE}},
                        {print, UnitType::INSTANCE}) { }
                        