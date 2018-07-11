#pragma once

#include "Entity.hpp"
#include "Key.hpp"

#include <tuple>

namespace ecs
{
    class EntityManager;
    class Key;


    ///
    /// A collection of entities
    ///
    class World
    {
        public:
            ///
            /// Constructor.
            ///
            World();

            ///
            /// Destructor.
            ///
            ~World();

            World(const World& other) = delete;
            World& operator=(const World& other) = delete;

            ///
            /// Create entity.
            /// Only entities created by this method are valid.
            /// \return The created entity.
            ///
            Entity create_entity();

            ///
            /// Destroy entity and all its attached components.
            /// All created entities can only be destroyed with
            /// this method.
            /// \param entity Entity to be destroyed.
            ///
            void destroy_entity(Entity& entity);

            ///
            /// Get an entity that matches key.
            /// If an entity has all the key types
            /// attached, it is a match. 
            /// \param with Key to match against.
            /// \param without Key to exclude with.
            /// \return Matching entity, or null entity if no match was found.
            ///
            Entity get_entity(const Key& with, const Key& without = Key());

            ///
            /// Get all entities that matches key.
            /// If an entity has all the key types
            /// attached, it is a match. 
            /// \param with Key to match against.
            /// \param without Key to exclude with.
            /// \return Matching entities.
            ///
            std::vector<Entity> get_entities(const Key& with, const Key& without = Key());
            
            ///
            /// Get all entities that have the same types of components 
            /// attached as another entity.
            /// \param entity Entity to match against.
            /// \return Matching entities.
            ///
            std::vector<Entity> get_entities(const Entity& entity);

            ///
            /// Get a component of type T that is attached
            /// to an entity that matches key.
            /// If an entity has all the key types
            /// attached, it is a match. 
            /// \tparam T Component type to return.
            /// \param with Key to match against.
            /// \param without Key to exclude with.
            /// \return Matching component, or nullptr if no match was found.
            ///
            template<typename T>
            T* get_component(const Key& with, const Key& without = Key()); 

            ///
            /// Get all components of type T that are attached
            /// to entities that match key.
            /// If an entity has all the key types
            /// attached, it is a match. 
            /// \tparam T Component type to return.
            /// \param with Key to match against.
            /// \param without Key to exclude with.
            /// \return Matching components.
            ///
            template<typename T>
            std::vector<T*> get_components(const Key& with, const Key& without = Key());

            ///
            /// Get components of types T that are attached to
            /// an entity that matches key.
            /// If an entity has all the key types
            /// attached, it is a match. 
            /// \tparam T Component types to return.
            /// \param with Key to match against.
            /// \param without Key to exclude with.
            /// \return Matching components, or nullptrs of no match was found.
            ///
            template<typename... T>
            std::tuple<T*...> get_component_tuple(const Key& with, const Key& without = Key()); 

            ///
            /// Get all components of types T that are attached to
            /// entities that match key.
            /// If an entity has all the key types
            /// attached, it is a match. 
            /// \tparam T Component types to return.
            /// \param with Key to match against.
            /// \param without Key to exclude with.
            /// \return Matching components, or nullptrs of no match was found.
            ///
            template<typename... T>
            std::vector<std::tuple<T*...>> get_component_tuples(const Key& with, const Key& without = Key()); 

            


            ///
            /// End current world tick.
            /// Clears all events. Invalidates all 
            /// pointers in events.
            ///
            void end_tick();

            ///
            /// Get entities destroyed since last clear_events.
            /// The returned entities are invalid handles
            /// and are only suitable for comparisons
            /// with other entities.
            /// \return Destroyed entities.
            ///
            const std::vector<ecs::Entity>& get_destroyed_entities() const;

            ///
            /// Get entities detached from component of type T
            /// since last end_tick.
            /// \tparam T Component type.
            /// \return Detached entities.
            ///
            template<typename T>
            const std::vector<ecs::Entity>& get_detached_entities() const;

            ///
            /// Get components detached since last end_tick.
            /// \tparam T Component type.
            /// \return Detached components.
            ///
            template<typename T>
            const std::vector<const T*>& get_detached_components() const;


            ///
            /// Get entities attached to component of type T
            /// since last end_tick.
            /// \tparam T Component type.
            /// \return Attached entities.
            ///
            template<typename T>
            const std::vector<ecs::Entity>& get_attached_entities() const;

            ///
            /// Get components attached since last end_tick.
            /// \tparam T Component type.
            /// \return Attached components.
            ///
            template<typename T>
            const std::vector<const T*>& get_attached_components() const;


            ///
            /// Detach all components of type T.
            /// \tparam T Component type.
            ///
            template<typename T>
            void detach_components();


        private:            
            Entity get_entity_with(const std::vector<TypeIndex>& ids);
            std::vector<Entity> get_entities_with(const std::vector<TypeIndex>& ids);
            void* get_component_with(TypeIndex type, std::vector<TypeIndex> with_types);
            std::vector<void*> get_components_with(TypeIndex type, std::vector<TypeIndex> with_types);

            std::vector<Entity> from_ids(const std::vector<EntityId>& ids);

            EntityManager* _man;
    };



}

#include "../../impl/World.inl"
