#include <ast/util.hpp>
#include <ast/identifier.hpp>
#include <ast/abstraction.hpp>
#include <ast/application.hpp>
#include <iostream>

namespace ast::util
{
    Node::Pointer replaceIdentifier(const Identifier::Pointer &id, const Node::Pointer &in, const Node::Pointer &by)
    {
        if (in->type() == NodeType::Identifier)
        {
            if (Node::cast<Identifier>(in)->name() == id->name())
                return by;
        }
        else if (in->type() == NodeType::Abstraction)
        {
            auto abs = Node::cast<Abstraction>(in);

            // Duplicated variable name inside abstraction
            if (abs->argument()->name() == id->name())
            {
                // Generate new name
                auto _x = Node::make<Identifier>(abs->argument()->name() + "\'");
                // Replace in body
                abs = Node::make<Abstraction>(_x, replaceIdentifier(abs->argument(), abs->body(), _x));
            }

            return Node::make<Abstraction>(abs->argument(), replaceIdentifier(id, abs->body(), by));
        }
        else if (in->type() == NodeType::Application)
        {
            auto app = Node::cast<Application>(in);

            return Node::make<Application>(replaceIdentifier(id, app->left(), by),
                                           replaceIdentifier(id, app->right(), by));
        }

        return in;
    }
}
