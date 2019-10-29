#ifndef LAMBDA_TYPE_HPP
#define LAMBDA_TYPE_HPP

#include "../common.hpp"

namespace ast
{
    class Type
    {
    public:
        using Pointer = std::shared_ptr<Type>;

        template<typename T>
        using PointerType = std::shared_ptr<T>;

        template<typename T>
        static PointerType<T> cast(Type::Pointer ref)
        {
            return std::dynamic_pointer_cast<T>(ref);
        }

        template<typename T, typename ...Args>
        static PointerType<T> make(Args &&... args)
        {
            return std::make_shared<T>(std::forward<Args>(args)...);
        }

        static bool equals(const Pointer &a, const Pointer &b)
        {
            return a->operator==(b);
        }

        static bool distinct(const Pointer &a, const Pointer &b)
        {
            return a->operator!=(b);
        }

    public:

        /**
         * Type equality by value
         */
        virtual bool operator ==(const Type &t) const = 0;

        inline bool operator !=(const Type &t) const
        {
            return !operator==(t);
        }

        inline bool operator ==(const Pointer &p) const
        {
            return p->operator==(*this);
        }

        inline bool operator !=(const Pointer &p) const
        {
            return p->operator!=(*this);
        }

        /**
         * @return A description of this type as string
         */
        virtual string toString() const = 0;
    };
}

#endif //LAMBDA_TYPE_HPP
