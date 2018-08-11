#include "EntityManager.hpp"
#include "../impl/TypeId.hpp"

#include <map>

namespace ecs
{

using namespace impl;


EntityManager::EntityManager()
: _next_entity_id(1)
{
    if (TypeId::sizes())
    {
        for(size_t size : *TypeId::sizes())
        {
            _stores.emplace_back(size);
        }
    }
}

EntityId EntityManager::create_entity()
{
    return _next_entity_id++;
}


void EntityManager::destroy_entity(EntityId id)
{
    _components.erase_entity(id);
}

void* EntityManager::attach(TypeIndex component, EntityId entity)
{
    ComponentStore* s = get_store(component);
    if(!s)
    {
        return nullptr;
    }

    void* component_id = s->insert();

    _components.entities.push_back(entity);
    _components.types.push_back(component);
    _components.components.push_back(component_id);
    return component_id;
}


EntityId EntityManager::get_entity(const Key& key) const
{
    size_t include_count = key.get_include_count();
    std::map<EntityId, size_t> num_matches;

    for(TypeIndex component = 0; component < key._includes.size(); component++)
    {
        if(!key._includes[component])
        {
            continue;
        }

        for(size_t i = 0; i < _components.types.size(); i++)
        {
            if(_components.types[i] == component)
            {
                if (++num_matches[_components.entities[i]] == include_count)
                {
                    return _components.entities[i];
                }
            }
        }    
    }

    return 0;
}

std::vector<EntityId> EntityManager::get_entities(const Key& key) const
{
    size_t include_count = key.get_include_count();
    std::map<EntityId, size_t> num_matches;

    std::vector<EntityId> matches;
    for(TypeIndex component = 0; component < key._includes.size(); component++)
    {
        if(!key._includes[component])
        {
            continue;
        }

        for(size_t i = 0; i < _components.types.size(); i++)
        {
            if (++num_matches[_components.entities[i]] == include_count)
            {
                matches.push_back(_components.entities[i]);
            }
        }    
    }

    return matches;
}

std::vector<EntityId> EntityManager::get_entities_like(EntityId entity) const
{
    std::vector<TypeIndex> components;
    for(size_t i = 0; i < _components.entities.size(); i++)
    {
        if(_components.entities[i] == entity)
        {
            components.push_back(i);
        }
    }

    for(TypeIndex& component : components)
    {
        component = _components.types[component];
    }

    return get_entities(Key::create(components));
}


bool EntityManager::detach(TypeIndex component, void* data, EntityId entity)
{
    ComponentStore* s = get_store(component);
    if(!s)
    {
        return false;
    }

    return _components.erase_component(data, entity);
}

std::vector<void*> EntityManager::detach(TypeIndex component, EntityId entity)
{
    ComponentStore* s = get_store(component);
    if(!s)
    {
        return {};
    }

    return _components.erase_type(component, entity);
}

void* EntityManager::get_component(TypeIndex component, EntityId entity) const
{
    for(size_t i = 0; i < _components.entities.size(); i++)
    {
        if(_components.entities[i] == entity &&
           _components.types[i] == component)
        {
            return _components.components[i];
        }
    }

    return nullptr;
}



void* EntityManager::get_component_with(TypeIndex type, Key key) const
{
    key._includes[type] = true;
    EntityId entity = get_entity(key);
    return get_component(type, entity);
}

std::vector<void*> EntityManager::get_components_with(TypeIndex type, Key key) const
{
    key._includes[type] = true;
    std::vector<EntityId> entities = get_entities(key);
    std::vector<void*> components;
    for(size_t i = 0, j = 0; i < entities.size() && j < _components.types.size(); j++)
    {
        if(_components.types[j] == type &&
            _components.entities[j] == entities[i])
        {
            i++;
            components.push_back(_components.components[j]);
        }
    }

    return components;
}


EntityManager::ComponentStore* EntityManager::get_store(TypeIndex id)
{
    if(id >= _stores.size())
    {
        return nullptr;
    }

    return &_stores[id];
}



EntityManager::ComponentStore::ComponentStore(size_t size)
: _size(size)
, _block_size(block_size())
, _block_cursor(_block_size)
{
}



void* EntityManager::ComponentStore::insert()
{
    if(!_free_ptrs.empty())
    {
        void* ptr = _free_ptrs.back();
        _free_ptrs.pop_back();
        return ptr;
    }

    if(_block_cursor == _block_size)
    {
        append_block();
        _block_cursor = _size;
        return _blocks.back()->data();
    }

    void* ptr = _blocks.back()->data() + _block_cursor;
    _block_cursor += _size;
    return ptr;
}

bool EntityManager::ComponentStore::erase(void* ptr)
{
    for(void* free_ptr : _free_ptrs)
    {
        if(ptr == free_ptr)
        {
            return false;
        }
    }

    for(const std::unique_ptr<std::vector<char>>& block : _blocks)
    {
        if(ptr >= block->data() &&
           (char*)ptr + _size <= block->data() + _block_size)
        {
            _free_ptrs.push_back(ptr);
            return true;
        }
    }

    return false;
}

void EntityManager::ComponentStore::append_block()
{
    _blocks.emplace_back(new std::vector<char>(_block_size));
}

size_t EntityManager::ComponentStore::block_size() const
{
    static constexpr size_t max_size = 4096;
    size_t size = (max_size / _size) * _size;
    if(size == 0)
    {
        size = _size;
    }

    return size;
}

bool EntityManager::ComponentManager::erase_entity(EntityId entity)
{
    std::vector<size_t> indices;
    for(size_t i = 0; i < entities.size(); i++)
    {
        if(entities[i] == entity)
        {
            indices.push_back(i);
        }
    }

    return erase(indices);
}

bool EntityManager::ComponentManager::erase_type(TypeIndex type)
{
    std::vector<size_t> indices;
    for(size_t i = 0; i < types.size(); i++)
    {
        if(types[i] == type)
        {
            indices.push_back(i);
        }
    }

    return erase(indices);
}


std::vector<void*> EntityManager::ComponentManager::erase_type(TypeIndex type, EntityId entity)
{
    std::vector<size_t> indices;
    for(size_t i = 0; i < types.size(); i++)
    {
        if(types[i] == type &&
           entities[i] == entity)
        {
            indices.push_back(i);
        }
    }

    std::vector<void*> erased;
    for(size_t i : indices)
    {
        erased.push_back(components[i]);
    }

    erase(indices);
    return erased;
}


bool EntityManager::ComponentManager::erase_component(void* component)
{
    std::vector<size_t> indices;
    for(size_t i = 0; i < components.size(); i++)
    {
        if(components[i] == component)
        {
            indices.push_back(i);
        }
    }

    return erase(indices);
}

bool EntityManager::ComponentManager::erase_component(void* component, EntityId entity)
{
    std::vector<size_t> indices;
    for(size_t i = 0; i < components.size(); i++)
    {
        if(components[i] == component &&
           entities[i] == entity)
        {
            indices.push_back(i);
        }
    }

    return erase(indices);
}

bool EntityManager::ComponentManager::erase(size_t index)
{
    entities[index] = entities.back();
    entities.pop_back();
    types[index] = types.back();
    types.pop_back();
    components[index] = components.back();
    components.pop_back();

    return true;
}

bool EntityManager::ComponentManager::erase(const std::vector<size_t>& indices)
{
    for(auto it = indices.rbegin(); it != indices.rend(); it++)
    {
        entities[*it] = entities.back();
        entities.pop_back();
    }

    for(auto it = indices.rbegin(); it != indices.rend(); it++)
    {
        types[*it] = types.back();
        types.pop_back();
    }

    for(auto it = indices.rbegin(); it != indices.rend(); it++)
    {
        components[*it] = components.back();
        components.pop_back();
    }

    return !indices.empty();
}


}