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

Entity World::get_entity(const Key& with, const Key& without)
{
    // TODO: Implement "without".
    return get_entity_with(with._types);
}

std::vector<Entity> World::get_entities(const Key& with, const Key& without) 
{ 
    // TODO: Implement "without".
    return get_entities_with(with._types); 
}

Entity World::get_entity_with(const std::vector<TypeIndex>& ids)
{
    return Entity(_man->get_entity_with(ids), *this, *_man);
}


std::vector<Entity> World::get_entities_with(const std::vector<TypeIndex>& ids)
{
    return from_ids(_man->get_entities_with(ids));
}

std::vector<Entity> World::get_entities(const Entity& entity)
{
    return from_ids(_man->get_entities_like(entity._id));
}

void* World::get_component_with(TypeIndex type, std::vector<TypeIndex> with_types)
{
    return _man->get_component_with(type, with_types);
}

std::vector<void*> World::get_components_with(TypeIndex type, std::vector<TypeIndex> with_types)
{
    return _man->get_components_with(type, with_types);
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

void World::end_tick()
{
    // TODO: Implement this method.
}

const std::vector<ecs::Entity>& World::get_destroyed_entities() const
{
    // TODO: Implement this method.
    return {};
}



}