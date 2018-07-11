#pragma once

#include <vector>
#include <memory>

#include "../impl/Ids.hpp"

namespace ecs
{
    using namespace impl;

class EntityManager
{
    public:
        EntityManager();

        EntityId create_entity();
        void destroy_entity(EntityId id);

        void* attach(TypeIndex component, EntityId entity);
        
        bool detach(TypeIndex component, void* data, EntityId entity);
        std::vector<void*> detach(TypeIndex component, EntityId entity);

        EntityId get_entity_with(std::vector<TypeIndex> components) const;
        std::vector<EntityId> get_entities_with(std::vector<TypeIndex> components) const;
        std::vector<EntityId> get_entities_like(EntityId entity) const;

        void* get_component(TypeIndex component, EntityId entity);
        void* get_component_with(TypeIndex type, std::vector<TypeIndex> with_types) const;
        std::vector<void*> get_components_with(TypeIndex type, std::vector<TypeIndex> with_types) const;

    private:
        class ComponentStore
        {
            public:
                ComponentStore(size_t size);

                void* insert();
                bool erase(void* ptr);

            private:
                void append_block();
                size_t block_size() const;

                size_t _size;
                size_t _block_size;
                size_t _block_cursor;
                std::vector<std::unique_ptr<std::vector<char>>> _blocks;
                std::vector<void*> _free_ptrs;
        };

        struct ComponentManager
        {
            std::vector<EntityId> entities;
            std::vector<TypeIndex> types;
            std::vector<void*> components;

            bool erase_entity(EntityId entity);
            bool erase_type(TypeIndex type);
            std::vector<void*> erase_type(TypeIndex type, EntityId entity);
            bool erase_component(void* component);
            bool erase_component(void* component, EntityId entity);

            private:
            bool erase(size_t index);
            bool erase(const std::vector<size_t>& indices);
        };


        ComponentStore* get_store(TypeIndex id);

        
        std::vector<ComponentStore> _stores;
        ComponentManager _components;
        
        EntityId _next_entity_id;

};

}