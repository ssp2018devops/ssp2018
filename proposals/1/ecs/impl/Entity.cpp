#include <ecs/Entity.hpp>
#include "../src/EntityManager.hpp"
#include <type_traits>

namespace ecs
{

using namespace impl;

Entity::Entity()
: _id(0)
, _world(nullptr)
{
}


Entity::Entity(EntityId id, World& world, EntityManager& man)
: _id(id)
, _world(&world)
, _man(&man)
{
}

bool Entity::is_null() const
{
    return _id == 0;
}

bool Entity::operator==(std::nullptr_t null_ptr) const
{
    return is_null();
}

bool Entity::operator!() const
{
    return is_null();
}

Entity::operator bool() const
{
    return !is_null();
}

Entity& Entity::operator=(std::nullptr_t null_ptr)
{
    _id = 0;
    return *this;
}

void* Entity::get(TypeIndex id) const
{
    return _man->get_component(id, _id);
}

void* Entity::attach(TypeIndex id)
{
    return _man->attach(id, _id);
}

bool Entity::detach(TypeIndex id, void* data)
{
    return _man->detach(id, data, _id);
}

std::vector<void*> Entity::detach(TypeIndex id)
{
    return _man->detach(id, _id);
}

Key Entity::get_key() const
{
    // TODO: Implememt this method.
    return Key();
}


World& Entity::get_world() const
{
    return *_world;
}

}