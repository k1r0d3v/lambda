#ifndef LAMBDA_TUPLE_TYPE_HPP
#define LAMBDA_TUPLE_TYPE_HPP

#include "../common.hpp"
#include "type.hpp"

namespace ast
{
    class TupleType : public Type
    {
    public:
        explicit TupleType(list<Type::Pointer> types)
                : mElements(std::move(types))
        { }

        const list<Type::Pointer> &elements() const {
            return mElements;
        }

        bool equals(const Type::Pointer &t) const override
        {
            auto tt = Type::cast<TupleType>(t);
            if (tt != nullptr)
            {
                if (tt->mElements.size() != mElements.size()) return false;

                for (size_t i = 0; i < mElements.size(); ++i)
                    if (Type::distinct(mElements[i], tt->mElements[i]))
                        return false;
                return true;
            }
            // TODO: Move this from here
            else if (Type::cast<DynType>(t) != nullptr)
                return true;

            return false;
        }

        Type::Pointer typeOfField(const string &name) override
        {
            int index;
            try
            {
                index = std::stoi(name);
                if (index < 0 || index >= mElements.size())
                    throw std::runtime_error("");
            }
            catch (const std::exception &e)
            {
                throw AttributeException("\'" + this->toString() + "\' object has no attribute \'" + name + "\'");
            }
            return mElements[index];
        }

        string toString() const override
        {
            string str = "{";
            for (const auto &i : mElements)
                str += i->toString() + ", ";

            if (!mElements.empty())
            {
                str.pop_back();
                str.pop_back();
            }

            str += "}";
            return str;
        }

    private:
        list<Type::Pointer> mElements;
    };
}


#endif //LAMBDA_TUPLE_TYPE_HPP
