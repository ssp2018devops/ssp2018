#pragma once

#include <ecs/Key.hpp>
#include "TypeId.hpp"

namespace ecs
{
    using namespace impl;

    template<typename... IncludeTypes>
    Key Key::create()
    {
        return create({TypeId::index<IncludeTypes>...});
    }


    template<typename... IncludeTypes>
    void Key::include()
    {
        include({TypeId::index<IncludeTypes>...});
    }

    template<typename... ExcludeTypes>
    void Key::exclude()
    {
        exclude({TypeId::index<ExcludeTypes>...});
    }
}