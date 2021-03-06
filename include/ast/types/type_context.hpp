#ifndef LAMBDA_TYPE_CONTEXT_HPP
#define LAMBDA_TYPE_CONTEXT_HPP

#include <map>
#include "../common.hpp"
#include "type.hpp"

namespace ast
{
    class TypeContext
    {
    private:
        using StrTypeMapType = std::map<string, Type::Pointer>;

    public:
        TypeContext() = default;
        /**
         * Set the type of id passed
         * @return type passed or nullptr if id not exist
         */
        Type::Pointer setTypeFor(const string& id, const Type::Pointer& value)
        {
            if (value != nullptr)
            {
                auto it = mMap.find(id);
                mMap[id] = value;

                if (it == mMap.end())
                    return Type::Pointer();
                return it->second;
            }
            else
            {
                mMap.erase(id);
                return Type::Pointer();
            }
        }
        /**
         * Get the type of id passed
         * @return the type of id or nullptr if id not exist
         */
        Type::Pointer getTypeOf(const string& id) const
        {
            auto it = mMap.find(id);
            if (it == mMap.end())
                return Type::Pointer();
            return it->second;
        }

    private:
        StrTypeMapType mMap;
    };
}

#endif //LAMBDA_TYPE_CONTEXT_HPP
