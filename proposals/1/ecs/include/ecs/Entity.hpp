#pragma once

#include <vector>

#include "Key.hpp"
#include "../../impl/Ids.hpp"

namespace ecs
{
    class World;
    class EntityManager;
    using namespace impl;

    ///
    /// A handle to an entity that resides in a World.
    /// An entity may have components attached to
    /// it of any type.
    ///
    class Entity
    {
        public:
            ///
            /// Constructor
            ///
            Entity();

            ///
            /// Get an attached component of type T.
            /// \tparam T Component type.
            /// \return A component, or nullptr if none is attached.
            ///
            template<typename T>
            operator T*();

            ///
            /// Get an attached component of type T.
            /// \tparam T Component type.
            /// \return A component, or nullptr if none is attached.
            /// 
            template<typename T>
            operator const T*() const;

            ///
            /// Get existing or attach component of type T.
            /// \tparam T Component type.
            /// \return An existing component, or a new if none was attached.
            ///
            template<typename T>
            operator T&();

            ///
            /// Check if a component of type T is attached.
            /// \tparam T Component type.
            /// \return True if attached, else false.
            ///
            template<typename T>
            bool has() const;

            ///
            /// Get an attached component of type T.
            /// \tparam T Component type.
            /// \return A component, or nullptr if none is attached.
            ///
            template<typename T>
            T* get();

            ///
            /// Get an attached component of type T.
            /// \tparam T Component type.
            /// \return A component, or nullptr if none is attached.
            ///
            template<typename T>
            const T* get() const;

            ///
            /// Check if null.
            /// \return True if null, else false.
            ///
            bool operator!() const;

            ///
            /// Check if null.
            /// \return True if null, else false.
            ///
            bool is_null() const;

            ///
            /// Check if null.
            /// \return True if null, else false.
            ///
            bool operator==(std::nullptr_t null_ptr) const;

            ///
            /// Check if not null.
            /// \return True if not null, else false.
            ///
            operator bool() const;

            ///
            /// Set to null.
            /// Does not destroy the entity.
            /// \param null_ptr A null pointer.
            /// \return This entity.
            ///
            Entity& operator=(std::nullptr_t null_ptr);

            ///
            /// Attach a component by copy.
            /// \tparam T Component type.
            /// \param component Component to attach.
            /// \return Attached component.
            ///
            template<typename T>
            T& operator+=(const T& component);

            ///
            /// Attach a component by copy.
            /// \tparam T Component type.
            /// \param component Component to attach.
            /// \return Attached component.
            ///
            template<typename T>
            T& attach(const T& component);

            ///
            /// Attach a component of type T.
            /// \tparam T Component type.
            /// \return Attached component.
            ///
            template<typename T>
            T& attach();

            ///
            /// Detach a component if attached.
            /// \tparam T Component type.
            /// \param component Component to detach.
            /// \return True if component was detached, else false.
            ///
            template<typename T>
            bool operator-=(T* component);

            ///
            /// Detach a component if attached.
            /// \tparam T Component type.
            /// \param component Component to detach.
            /// \return True if component was detached, else false.
            ///
            template<typename T>
            bool detach(T* component);
            
            ///
            /// Detach all components of type T if attached.
            /// \tparam T Component type.
            /// \return True components were detached, else false.
            ///
            template<typename T>
            bool detach();


            ///
            /// Get key that describes this entity.
            /// \return The key.
            ///
            Key get_key() const;

            ///
            /// Get the world this entity lives in.
            /// \return The world.
            ///
            World& get_world() const;

        private:
            friend World;
            Entity(EntityId id, World& world, EntityManager& man);
            
            void* get(TypeIndex id) const;
            void* attach(TypeIndex id);
            bool detach(TypeIndex id, void* data);
            std::vector<void*> detach(TypeIndex id);

            EntityId _id;
            World* _world;
            EntityManager* _man;
    };
}


#include "../../impl/Entity.inl"
