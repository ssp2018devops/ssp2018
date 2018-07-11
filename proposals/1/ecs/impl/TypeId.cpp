#include "TypeId.hpp"

#include "../src/EntityManager.hpp"

#include <type_traits>
#include <cassert>


namespace ecs
{
namespace impl
{


const std::vector<size_t>* TypeId::_sizes = nullptr;

TypeIndex TypeId::register_type(size_t size)
{
    static std::vector<size_t> sizes;
    _sizes = &sizes;
    sizes.push_back(size);
    assert(sizes.size() <= TypeIndex(-1));
    return (TypeIndex)(sizes.size() - 1);
}

const std::vector<size_t>* TypeId::sizes()
{
    return _sizes;
}


}

}