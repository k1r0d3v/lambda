#ifndef LAMBDA_TYPE_HPP
#define LAMBDA_TYPE_HPP

#include "common.hpp"

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


    public:
        virtual bool equals(Type::Pointer t) const = 0;

        bool distinct(Type::Pointer t) const {
            return !this->equals(std::move(t));
        }

        virtual string toString() const = 0;
    };
}

#endif //LAMBDA_TYPE_HPP
