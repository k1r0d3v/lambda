#ifndef LAMBDA_TYPE_HPP
#define LAMBDA_TYPE_HPP

#include <ast/common.hpp>

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

        static bool equals(const Type::Pointer &a, const Type::Pointer &b);

        static bool distinct(const Type::Pointer &a, const Type::Pointer &b);

    public:
        explicit Type(int kind);

        virtual bool equals(const Type::Pointer &t) const = 0;

        virtual string toString() const = 0;

        int kind() const { return mKind; }

    private:
        int mKind;
    };
}

#endif //LAMBDA_TYPE_HPP
