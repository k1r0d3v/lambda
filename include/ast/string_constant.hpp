#ifndef LAMBDA_STRING_CONSTANT_HPP
#define LAMBDA_STRING_CONSTANT_HPP

#include <ast/types/str_type.hpp>
#include "common.hpp"
#include "node.hpp"
#include "node_type.hpp"

namespace ast
{
    class StringConstant : public Node
    {
    public:
        using Pointer = Node::PointerType<StringConstant>;

    public:
        explicit StringConstant(string value)
                : Node(NodeType::StringConstant), mValue(std::move(value))
        { }

        const string &value() const
        {
            return mValue;
        }

        Node::Pointer evaluate(const Node::Pointer &self, Context &context) const override
        {
            return self;
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            return StrType::INSTANCE;
        }

        Node::Pointer copy() const override
        {
            return Node::make<StringConstant>(mValue);
        }

        string toString() const override
        {
            return "\"" + scape() + "\"";
        }

    private:
        string scape() const
        {
            std::string s;

            for(auto c : mValue)
            {
                // check if a given character is printable
                // the cast is necessary to avoid undefined behaviour
                if(isprint((unsigned char)c))
                {
                    s += c;
                }
                else
                {
                    std::stringstream stream;
                    // if the character is not printable
                    // we'll convert it to a hex string using a stringstream
                    // note that since char is signed we have to cast it to unsigned first
                    stream << std::hex << (unsigned int)(unsigned char)(c);
                    std::string code = stream.str();
                    s += std::string("\\x")+(code.size()<2?"0":"")+code;
                }
            }

            return s;
        }

    private:
        string mValue;
    };
}

#endif //LAMBDA_STRING_CONSTANT_HPP
