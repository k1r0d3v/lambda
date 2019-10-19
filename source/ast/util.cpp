#include <ast/util.hpp>
#include <ast/identifier.hpp>
#include <ast/abstraction.hpp>
#include <ast/application.hpp>
#include <iostream>

namespace ast::util
{
    Node::Reference replaceIdentifier(Identifier::Reference id, Node::Reference in, Node::Reference by)
    {
        if (in->type() == NodeType::Identifier)
        {
            if (Node::castRef<Identifier>(in)->name() == id->name())
                return by;
        } else if (in->type() == NodeType::Abstraction)
        {
            auto abs = Node::castRef<Abstraction>(in);

            // Duplicated variable name inside abstraction
            if (abs->argument()->name() == id->name())
            {
                // Generate new name
                auto _x = Node::make<Identifier>(abs->argument()->name() + "\'");
                // Replace in body
                abs = Node::make<Abstraction>(_x, replaceIdentifier(abs->argument(), abs->body(), _x));
            }

            return Node::make<Abstraction>(abs->argument(), replaceIdentifier(id, abs->body(), by));
        } else if (in->type() == NodeType::Application)
        {
            auto app = Node::castRef<Application>(in);

            return Node::make<Application>(replaceIdentifier(id, app->left(), by),
                                           replaceIdentifier(id, app->right(), by));
        }

        return in;
    }

    bool isValueType(const Node &node)
    {
        return node.type() == NodeType::Abstraction;
    }
}
