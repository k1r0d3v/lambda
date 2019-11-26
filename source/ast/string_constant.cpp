#include <ast/string_constant.hpp>
#include <ast/node_kind.hpp>
#include <ast/types/str_type.hpp>
#include <ast/node_visitor.hpp>


using namespace ast;

static string stringScape(const string &str)
{
    std::string s;

    for(auto c : str)
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

StringConstant::StringConstant(string value)
    : Node(NodeKind::StringConstant), mValue(std::move(value)) { }

Node::Pointer StringConstant::evaluate(Context &context) const
{
    return this->copy();
}

Type::Pointer StringConstant::typecheck(TypeContext &context)
{
    return StrType::INSTANCE;
}

Node::Pointer StringConstant::copy() const
{
    return Node::make<StringConstant>(mValue);
}

string StringConstant::toString() const
{
    return "\"" + stringScape(mValue) + "\"";
}

Node::Pointer StringConstant::transform(NodeVisitor *visitor)
{
    return visitor->visitString(this);
}

