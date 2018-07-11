#pragma once

#include <ecs/Entity.hpp>
#include "TypeId.hpp"

namespace ecs
{

using namespace impl;


template<typename T>
Entity::operator T*()
{
    return get<T>();
}

template<typename T>
Entity::operator const T*() const 
{ 
    return get<T>();
}

template<typename T>
Entity::operator T&()
{
    T* t = get<T>();
    if(!t)
    {
        t = &attach<T>();
    }
    return *t;
}

template<typename T>
T* Entity::get()
{
    return (T*)get(TypeId::index<T>); 
}

template<typename T>
const T* Entity::get() const
{
    return (const T*)get(TypeId::index<T>); 
}

template<typename T>
bool Entity::has() const 
{ 
    return get(TypeId::index<T>); 
}

template<typename T>
T& Entity::operator+=(const T& component) 
{ 
    return attach(component); 
}


template<typename T>
T& Entity::attach(const T& component) 
{
    return *new(attach(TypeId::index<T>)) T(component);
}

template<typename T>
T& Entity::attach_move(T&& component)
{
    return *new(attach(TypeId::index<T>)) T(std::move(component));
}

template<typename T>
T& Entity::attach() 
{ 
    return *new(attach(TypeId::index<T>)) T();
}

template<typename T>
bool Entity::operator-=(T* component) 
{ 
    return detach(component); 
}

template<typename T>
bool Entity::detach(T* component) 
{ 
    if(!detach(TypeId::index<T>, component))
    {
        return false;
    }

    component->~T();
    return true;
}

template<typename T>
bool Entity::detach() 
{ 
    std::vector<void*> ptrs = detach(TypeId::index<T>);
    if(ptrs.empty())
    {
        return false;
    }

    for(void* ptr : ptrs)
    {
        ((T*)ptr)->~T();
    }
    
    return true;
}


}
