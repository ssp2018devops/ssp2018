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
    /// the key's includes.
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
            /// Get amount of types included.
            /// \return Include count.
            ///
            size_t get_include_count() const;


        private:
            friend class EntityManager;

            static Key create(const std::vector<TypeIndex>& includes);
            void include(const std::vector<TypeIndex>& includes);


            std::vector<bool> _includes;
    };

}


#include "../../impl/Key.inl"