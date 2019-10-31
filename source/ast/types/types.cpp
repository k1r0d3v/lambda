#include <ast/types/nat_type.hpp>
#include <ast/types/bool_type.hpp>
#include <ast/types/arrow_type.hpp>
#include <ast/types/unit_type.hpp>
#include <ast/types/str_type.hpp>

using namespace ast;

const Type::PointerType<NatType> NatType::INSTANCE = Type::make<NatType>();
const Type::PointerType<BoolType> BoolType::INSTANCE = Type::make<BoolType>();
const Type::PointerType<StrType> StrType::INSTANCE = Type::make<StrType>();
const Type::PointerType<UnitType> UnitType::INSTANCE = Type::make<UnitType>();
const Type::PointerType<ArrowType> ArrowType::NAT_NAT = Type::make<ArrowType>(NatType::INSTANCE, NatType::INSTANCE);
const Type::PointerType<ArrowType> ArrowType::NAT_BOOL = Type::make<ArrowType>(NatType::INSTANCE, BoolType::INSTANCE);