#pragma once

#include <vector>

#include "Ids.hpp"

namespace ecs
{

namespace impl
{
    class TypeId
    {
        public:
            static const std::vector<size_t>* sizes();
            
            template<typename T>
            static TypeIndex index;

        private:
            static TypeIndex register_type(size_t size);

            static const std::vector<size_t>* _sizes;
    };

    template<typename T>
    TypeIndex TypeId::index = register_type(sizeof(T));


    // template<typename T>
    // TypeId register_type()
    // { 
    //     return register_type(&_type_id<T>, sizeof(T));
    // }
    // TypeId register_type(TypeId id, size_t size);

    // template<typename T>
    // TypeId _type_id = register_type<T>();

    // struct Type
    // {
    //     TypeId id;
    //     size_t size;
    // };

    // class RegisteredTypes
    // {
    //     public:
    //         static std::vector<Type> types;
    // };
}

}