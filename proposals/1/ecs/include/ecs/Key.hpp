#pragma once

#include "../../impl/Ids.hpp"


#include <vector>

namespace ecs
{
    using namespace impl;

    class Key
    {
        public:
            Key();

            template<typename... T>
            static Key create();


        private:
            friend class World;
            friend class Entity;

            static Key create(const std::vector<TypeIndex>& types);


            std::vector<bool> _bits;
            std::vector<TypeIndex> _types;
    };

}


#include "../../impl/Key.inl"