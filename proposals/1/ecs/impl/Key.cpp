#include <ecs/Key.hpp>
#include "TypeId.hpp"

#include <cassert>

namespace ecs
{

using namespace impl;

Key::Key()
{
    _bits.resize(TypeId::sizes()->size());
}

Key Key::create(const std::vector<TypeIndex>& types)
{
    Key key;
    assert(key._bits.size() == TypeId::sizes()->size());

    for(TypeIndex i : types)
    {
        key._bits[i] = true;
    }

    key._types = types;

    return key;
}



}