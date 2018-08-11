#include <ecs/Key.hpp>
#include "TypeId.hpp"

#include <cassert>
#include <algorithm>

namespace ecs
{

using namespace impl;

Key::Key()
{
    _includes.resize(TypeId::sizes()->size());
}

Key Key::create(const std::vector<TypeIndex>& includes)
{
    Key key;
    assert(key._includes.size() == TypeId::sizes()->size());

    key.include(includes);

    return key;
}

void Key::include(const std::vector<TypeIndex>& includes)
{
    std::fill(_includes.begin(), _includes.end(), false);

    for(TypeIndex i : includes)
    {
        _includes[i] = true;
    }
}

size_t Key::get_include_count() const
{
    return std::count(_includes.begin(), _includes.end(), true);
}

}