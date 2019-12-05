#ifndef LAMBDA_TYPE_HPP
#define LAMBDA_TYPE_HPP

#include <ast/common.hpp>

namespace ast
{
    class TypeContext;

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
        explicit Type(int kind) : mKind(kind) { }
        /**
         * TODO
         */
        virtual void resolve(TypeContext &context) { }
        /**
         * Return true if this type is the same of type passed
         * @return bool
         */
        virtual bool isTypeOf(const Type::Pointer &t) const = 0;
        /**
         *  Return true if this type is a subtype of type passed
         * @return bool
         */
        virtual bool isSubtypeOf(const Type::Pointer &t) const = 0;
        /**
         *  Transform the values of class in string to view in output
         * @return string
         */
        virtual string toString() const = 0;

        int kind() const { return mKind; }

    private:
        int mKind;
    };
}

#endif //LAMBDA_TYPE_HPP