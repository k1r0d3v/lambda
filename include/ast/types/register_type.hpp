#ifndef LAMBDA_REGISTER_TYPE_HPP
#define LAMBDA_REGISTER_TYPE_HPP

#include "../common.hpp"
#include "type.hpp"

namespace ast
{
    class RegisterType : public Type
    {
    public:
        explicit RegisterType(std::map<string, Type::Pointer> types)
                : mElements(std::move(types))
        { }

        const std::map<string, Type::Pointer> &elements() const {
            return mElements;
        }

        bool equals(const Type::Pointer &t) const override
        {
            auto tt = Type::cast<RegisterType>(t);
            if (tt != nullptr)
            {
                if (tt->mElements.size() != mElements.size()) return false;

                for (const auto &i : mElements)
                {
                    // Search the entries
                    auto otherValue = tt->mElements.find(i.first);
                    auto myValue = mElements.find(i.first);

                    // Entry not found
                    if (otherValue == tt->mElements.end()) return false;

                    // Distinct type of entries
                    if (Type::distinct(myValue->second, otherValue->second))
                        return false;
                }
                return true;
            }
            // TODO: Move this from here
            else if (Type::cast<DynType>(t) != nullptr)
                return true;

            return false;
        }

        Type::Pointer typeOfField(const string &name) override
        {
            auto value = mElements.find(name);
            if (value == mElements.end())
                throw AttributeException("\'" + this->toString() + "\' object has no attribute \'" + name + "\'");
            return value->second;
        }

        string toString() const override
        {
            string str = "{";
            for (const auto &i : mElements)
                str += i.first + ": " + i.second->toString() + ", ";

            if (!mElements.empty())
            {
                str.pop_back();
                str.pop_back();
            }

            str += "}";
            return str;
        }

    private:
        std::map<string, Type::Pointer> mElements;
    };
}


#endif //LAMBDA_REGISTER_TYPE_HPP
