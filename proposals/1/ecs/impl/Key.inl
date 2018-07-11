#pragma once

#include <ecs/Key.hpp>
#include "TypeId.hpp"

namespace ecs
{
    using namespace impl;

    template<typename... T>
    Key Key::create()
    {
        return create({TypeId::index<T>...});
    }


}