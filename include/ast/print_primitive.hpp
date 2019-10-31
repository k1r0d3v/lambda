#ifndef LAMBDA_PRINT_PRIMITIVE_HPP
#define LAMBDA_PRINT_PRIMITIVE_HPP

#include "types/unit_type.hpp"
#include <iostream>
#include <ast/types/str_type.hpp>
#include "native_application.hpp"
#include "unit.hpp"

namespace ast
{
    class Print : public NativeApplication
    {
    public:
        using Pointer = Node::PointerType<Print>;

    private:
        static Node::Pointer print(const list<NativeArgument> &args)
        {
            auto arg = args[0].value;

            if (arg->nodeType() == NodeType::StringConstant)
            {
                auto str = Node::cast<StringConstant>(arg);
                std::cout << str->value();
            }
            else if (arg->nodeType() == NodeType::NaturalConstant)
            {
                auto nat = Node::cast<NaturalConstant>(arg);
                std::cout << nat->value();
            }
            else std::cout << arg->toString();

            return Node::make<Unit>();
        }

    public:
        explicit Print(Node::Pointer argument)
                : NativeApplication("print",
                                    {{std::move(argument), StrType::INSTANCE}},
                                    {print, UnitType::INSTANCE}) { }

    };
}

#endif //LAMBDA_PRINT_PRIMITIVE_HPP
