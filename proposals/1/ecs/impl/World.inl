#pragma once

#include <ecs/World.hpp>
#include <ecs/Key.hpp>
#include "TypeId.hpp"

namespace ecs
{

using namespace impl;




template<typename ComponentT>
ComponentT* World::get_component(const Key& key)
{
    return (ComponentT*)get_component_with(TypeId::index<ComponentT>, key);
}

template<typename ComponentT>
std::vector<ComponentT*> World::get_components(const Key& key)
{
    std::vector<void*> result = get_components_with(TypeId::index<ComponentT>, key);

    std::vector<ComponentT*> components;
    for(void* c : result)
    {
        components.push_back((ComponentT*)c);
    }

    return components;
}

template<typename... T>
std::tuple<T*...> World::get_component_tuple(const Key& key)
{
    // TODO: Implement this method.
    // TODO: Implement "without".
    return std::make_tuple<T*...>();
}

template<typename... T>
std::vector<std::tuple<T*...>> World::get_component_tuples(const Key& key)
{
    // TODO: Implement method.
    // TODO: Implement "without".
    return {};
}

template<typename T>
const std::vector<ecs::Entity>& World::get_detached_entities() const
{
    // TODO: Implement this method.
    return std::vector<ecs::Entity>();
}


template<typename T>
const std::vector<const T*>& World::get_detached_components() const
{
    // TODO: Implement this method.
    return std::vector<const T*>();
}


template<typename T>
const std::vector<ecs::Entity>& World::get_attached_entities() const
{
    // TODO: Implement this method.
    return std::vector<ecs::Entity>();
}


template<typename T>
const std::vector<const T*>& World::get_attached_components() const
{
    // TODO: Implement this method.
    return std::vector<const T*>();
}

template<typename T>
void World::detach_components()
{
    // TODO: Implement method.
}

}