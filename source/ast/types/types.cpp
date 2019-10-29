#include <ast/types/nat_type.hpp>
#include <ast/types/bool_type.hpp>
#include <ast/types/arrow_type.hpp>
#include <ast/types/unit_type.hpp>

using namespace ast;

const Type::PointerType<NatType> NatType::NAT = Type::make<NatType>();
const Type::PointerType<BoolType> BoolType::BOOL = Type::make<BoolType>();
const Type::PointerType<UnitType> UnitType::UNIT = Type::make<UnitType>();
const Type::PointerType<ArrowType> ArrowType::NAT_NAT = Type::make<ArrowType>(NatType::NAT, NatType::NAT);
const Type::PointerType<ArrowType> ArrowType::NAT_BOOL = Type::make<ArrowType>(NatType::NAT, BoolType::BOOL);