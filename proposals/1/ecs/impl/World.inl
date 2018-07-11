#pragma once

#include <ecs/World.hpp>
#include <ecs/Key.hpp>
#include "TypeId.hpp"

namespace ecs
{

using namespace impl;




template<typename ComponentT>
ComponentT* World::get_component(const Key& with, const Key& without)
{
    // TODO: Implement "without".
    return (ComponentT*)get_component_with(TypeId::index<ComponentT>, with._types);
}

template<typename ComponentT>
std::vector<ComponentT*> World::get_components(const Key& with, const Key& without)
{
    // TODO: Implement "without".
    std::vector<void*> result = get_components_with(TypeId::index<ComponentT>, with._types);

    std::vector<ComponentT*> components;
    for(void* c : result)
    {
        components.push_back((ComponentT*)c);
    }

    return components;
}

template<typename... T>
std::tuple<T*...> World::get_component_tuple(const Key& with, const Key& without)
{
    // TODO: Implement this method.
    // TODO: Implement "without".
    return std::make_tuple<T*...>();
}

template<typename... T>
std::vector<std::tuple<T*...>> World::get_component_tuples(const Key& with, const Key& without)
{
    // TODO: Implement method.
    // TODO: Implement "without".
    return {};
}


template<typename T>
const std::vector<const void*>& World::get_detached_components(const Entity& entity) const
{
    // TODO: Implement method.
    return std::vector<const void*>();
}



}