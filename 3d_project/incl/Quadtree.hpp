/****************************************************************
****************************************************************
*
* {{PROJECT_NAME}}
* {{PROJECT_DESCRIPTION}}
* Mikael Hernvall (mikael@hernvall.com)
* 2016
*
****************************************************************
****************************************************************/

#ifndef QUADTREE_HPP
#define QUADTREE_HPP

////////////////////////////////////////////////
// Simple DirectMedia Layer (SDL)
#include "glm/vec3.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// C++ Standard Library
#include <list>
#include <set>
////////////////////////////////////////////////

class Quadtree
{
    private:
        struct Object
        {
            void* pointer;
            glm::vec3 position;
            float radius;
        };


    public:
        Quadtree(glm::vec3 position, float width, float height);

        void clear();
        void insert(void* object, glm::vec3 position, float radius);


        template<typename... T>
        std::set<void*> getNearbyObjects(T... t) const
        {
            std::set<void*> nearbyObjects;
            getNearbyObjects(nearbyObjects, t...);

            return nearbyObjects;
        }

//        std::set<void*> getNearbyObjects(glm::vec3 position, float radius) const;
//
//        std::set<void*> getNearbyObjects(glm::vec3 eye, glm::vec3 leftEdge, glm::vec3 rightEdge, float farDistanceSqrd) const;

        ////////////////////////////////////////////////
        // DEBUGGING
        struct Quad
        {
            glm::vec3 position;
            float width;
            float height;
        };

//        std::list<Quad> getIntersectingQuads(glm::vec3 position, float radius) const;
//        std::list<Quad> getIntersectingQuads(glm::vec3 eye, glm::vec3 leftEdge, glm::vec3 rightEdge, float farDistanceSqrd) const;
//        std::list<Quad> getIntersectingQuads(glm::vec3 topLeft, glm::vec3 right, glm::vec3 down) const;

        template<typename... T>
        std::list<Quad> getIntersectingQuads(T... t) const
        {
            std::list<Quad> quads;
            getIntersectingQuads(quads, t...);

            return quads;
        }

        void getQuads(std::list<Quad>& quads) const;
        ////////////////////////////////////////////////

    private:
        Quadtree(size_t level, glm::vec3 position, float width, float height, const std::list<const Object*>& objects);

        void insert(const Object* object);
        void split();
        bool isOutOfBounds(glm::vec3 position, float radius) const;
        bool isOutOfBounds(glm::vec3 eye, glm::vec3 leftEdge, glm::vec3 rightEdge, float farDistanceSqrd) const;
        bool isOutOfBounds(glm::vec3 topLeft, glm::vec3 right, glm::vec3 down) const;
        bool isPointInBounds(glm::vec3 p) const;


        template<typename... T>
        void getNearbyObjects(std::set<void*>& nearbyObjects, T... t) const
        {
            if(isOutOfBounds(t...))
                return;

            if(mChildren.empty())
            {
                for(const Object* object : mObjects)
                    nearbyObjects.insert(object->pointer);
            }
            else
            {
                for(const Quadtree& child : mChildren)
                    child.getNearbyObjects(nearbyObjects, t...);
            }
        }

//        void getNearbyObjects(glm::vec3 position, float radius, std::set<void*>& nearbyObjects) const;
//        void getNearbyObjects(glm::vec3 eye, glm::vec3 leftEdge, glm::vec3 rightEdge, float farDistanceSqrd, std::set<void*>& nearbyObjects) const;

        ////////////////////////////////////////////////
        // DEBUGGING
        template<typename... T>
        void getIntersectingQuads(std::list<Quad>& quads, T... t) const
        {
            if(isOutOfBounds(t...))
                return;

            if(mChildren.empty())
            {
                Quad quad;
                quad.height = M_HEIGHT;
                quad.position = M_POSITION;
                quad.width = M_WIDTH;
                quads.push_back(quad);
            }
            else
            {
                for(const Quadtree& child : mChildren)
                    child.getIntersectingQuads(quads, t...);
            }
        }


//        void getIntersectingQuads(glm::vec3 position, float radius, std::list<Quad>& quads) const;
//        void getIntersectingQuads(glm::vec3 eye, glm::vec3 leftEdge, glm::vec3 rightEdge, float farDistanceSqrd, std::list<Quad>& quads) const;
//        void getIntersectingQuads(glm::vec3 topLeft, glm::vec3 right, glm::vec3 down, std::list<Quad>& quads) const;
        ////////////////////////////////////////////////

    private:
        static const size_t MAX_OBJECTS = 30;
        static const size_t MAX_LEVELS = 6;

        const size_t M_LEVEL;
        const glm::vec3 M_POSITION;
        const float M_WIDTH;
        const float M_HEIGHT;

        std::list<Quadtree>         mChildren;
        std::list<Object>           mRootObjects; // Only contains objects if this is the quadtree's root.
        std::list<const Object*>          mObjects;
};

#endif //QUADTREE_HPP
