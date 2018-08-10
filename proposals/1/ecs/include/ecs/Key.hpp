#pragma once

#include "../../impl/Ids.hpp"


#include <vector>

namespace ecs
{
    using namespace impl;

    ///
    /// Describes an entity by its attached component types.
    /// Used as a search query to find matching entities
    /// by including entities with types that match
    /// the key's includes, and excluding entities that match
    /// the key's excludes.
    ///
    class Key
    {
        public:
            ///
            /// Constructor
            /// Creates an empty key.
            ///
            Key();

            ///
            /// Create a key with includes.
            /// \tparam IncludeTypes Types to include.
            /// \return The created key.
            ///
            template<typename... IncludeTypes>
            static Key create();

            ///
            /// Set includes.
            /// \tparam IncludeTypes Types to include.
            ///
            template<typename... IncludeTypes>
            void include();

            ///
            /// Set excludes.
            /// \tparam ExcludeTypes Types to exclude.
            ///
            template<typename... ExcludeTypes>
            void exclude();

        private:
            friend class World;
            friend class Entity;

            static Key create(const std::vector<TypeIndex>& types);
            void include(const std::vector<TypeIndex>& types);
            void exclude(const std::vector<TypeIndex>& types);


            std::vector<bool> _bits;
            std::vector<TypeIndex> _types;
    };

}


#include "../../impl/Key.inl"