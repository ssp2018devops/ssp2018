#include <ecs/World.hpp>
#include "../src/EntityManager.hpp"

namespace ecs
{

World::World()
: _man(new EntityManager())
{

}

World::~World()
{
    delete _man;
}

Entity World::create_entity()
{
    return Entity(_man->create_entity(), *this, *_man);
}

void World::destroy_entity(Entity& entity)
{
    _man->destroy_entity(entity._id);
    entity._id = 0;    
}

Entity World::get_entity(const Key& key)
{
    return Entity(_man->get_entity(key), *this, *_man);
}

std::vector<Entity> World::get_entities(const Key& key) 
{ 
    return from_ids(_man->get_entities(key));
}

std::vector<Entity> World::get_entities(const Entity& entity)
{
    return from_ids(_man->get_entities_like(entity._id));
}

void* World::get_component_with(TypeIndex type, const Key& key)
{
    return _man->get_component_with(type, key);
}

std::vector<void*> World::get_components_with(TypeIndex type, const Key& key)
{
    return _man->get_components_with(type, key);
}

std::vector<Entity> World::from_ids(const std::vector<EntityId>& ids)
{
    std::vector<Entity> entities;
    entities.reserve(ids.size());
    for(EntityId id : ids)
    {
        entities.emplace_back(Entity(id, *this, *_man));
    }

    return entities;
}

void World::swap_events()
{
    // TODO: Implement this method.
}

const std::vector<ecs::Entity>& World::get_destroyed_entities() const
{
    // TODO: Implement this method.
    return {};
}



}