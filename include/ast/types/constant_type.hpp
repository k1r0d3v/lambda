#ifndef LAMBDA_CONSTANT_TYPE_HPP
#define LAMBDA_CONSTANT_TYPE_HPP

#include "../common.hpp"
#include "type.hpp"

namespace ast
{
    class ConstantType : public Type
    {
    public:
        using Pointer = Type::PointerType<ConstantType>;

    public:
        explicit ConstantType(const string &name) : mName(name) { }

        bool operator==(const Type &t) const override
        {
            auto ct = dynamic_cast<const ConstantType *>(&t);
            if (ct != nullptr)
                return mName == ct->mName;
            return false;
        }

        const char *name() const {
            return mName.data();
        }

        string toString() const override
        {
            return mName;
        }

    private:
        string mName;
    };
}

#endif //LAMBDA_CONSTANT_TYPE_HPP
