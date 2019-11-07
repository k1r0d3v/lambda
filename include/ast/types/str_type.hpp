#ifndef LAMBDA_STR_TYPE_HPP
#define LAMBDA_STR_TYPE_HPP

#include <ast/types/constant_type.hpp>
#include <ast/types/type_kind.hpp>

namespace ast
{
    class StrType : public ConstantType
    {
    public:
        static const Type::PointerType<StrType> INSTANCE;

    public:
        explicit StrType() : ConstantType(TypeKind::Str, "Str") { }
    };
}

#endif //LAMBDA_STR_TYPE_HPP
