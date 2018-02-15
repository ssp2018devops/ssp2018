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

////////////////////////////////////////////////
// Internal headers
#include "Quadtree.hpp"
////////////////////////////////////////////////

#include "glm/geometric.hpp"

Quadtree::Quadtree(glm::vec3 position, float width, float height)
: M_LEVEL(0)
, M_POSITION(position)
, M_WIDTH(width)
, M_HEIGHT(height)
{
}

Quadtree::Quadtree(size_t level, glm::vec3 position, float width, float height, const std::list<const Object*>& objects)
: M_LEVEL(level)
, M_POSITION(position)
, M_WIDTH(width)
, M_HEIGHT(height)
{
    for(const Object* object : objects)
        insert(object);
}


void Quadtree::clear()
{
    mChildren.clear();
    mObjects.clear();
    mRootObjects.clear();
}

void Quadtree::insert(void* object, glm::vec3 position, float radius)
{
    if(isOutOfBounds(position, radius))
        return;

    mRootObjects.push_back({object, position, radius});
    mObjects.push_back(&mRootObjects.back());

    if(mChildren.empty())
    {
        if(mObjects.size() > MAX_OBJECTS && M_LEVEL < MAX_LEVELS)
            split();
    }
    else
        for(Quadtree& child : mChildren)
            child.insert(mObjects.back());
}

//
//std::set<void*> Quadtree::getNearbyObjects(glm::vec3 position, float radius) const
//{
//    std::set<void*> nearbyObjects;
//    getNearbyObjects(position, radius, nearbyObjects);
//
//    return nearbyObjects;
//}

//void Quadtree::getNearbyObjects(glm::vec3 position, float radius, std::set<void*>& nearbyObjects) const
//{
//    if(isOutOfBounds(position, radius))
//        return;
//
//    if(mChildren.empty())
//    {
//        for(const Object* object : mObjects)
//            nearbyObjects.insert(object->pointer);
//    }
//    else
//    {
//        for(const Quadtree& child : mChildren)
//            child.getNearbyObjects(position, radius, nearbyObjects);
//    }
//}


//std::set<void*> Quadtree::getNearbyObjects(glm::vec3 eye, glm::vec3 leftEdge, glm::vec3 rightEdge, float farDistanceSqrd) const
//{
//    std::set<void*> nearbyObjects;
//    getNearbyObjects(eye, leftEdge, rightEdge, farDistanceSqrd, nearbyObjects);
//
//    return nearbyObjects;
//}

//void Quadtree::getNearbyObjects(glm::vec3 eye, glm::vec3 leftEdge, glm::vec3 rightEdge, float farDistanceSqrd, std::set<void*>& nearbyObjects) const
//{
//    if(isOutOfBounds(eye, leftEdge, rightEdge, farDistanceSqrd))
//        return;
//
//
//    if(mChildren.empty())
//    {
//        for(const Object* object : mObjects)
//            nearbyObjects.insert(object->pointer);
//    }
//    else
//    {
//        for(const Quadtree& child : mChildren)
//            child.getNearbyObjects(eye, leftEdge, rightEdge, farDistanceSqrd, nearbyObjects);
//    }
//}


void Quadtree::insert(const Object* object)
{
    if(isOutOfBounds(object->position, object->radius))
        return;

    mObjects.push_back(object);

    if(mChildren.empty())
    {
        if(mObjects.size() > MAX_OBJECTS && M_LEVEL < MAX_LEVELS)
            split();
    }
    else
        for(Quadtree& child : mChildren)
            child.insert(mObjects.back());
}

void Quadtree::split()
{
    float childWidth = M_WIDTH / 2;
    float childHeight = M_HEIGHT / 2;

    glm::vec3 childPosition = M_POSITION;

    mChildren.push_back(Quadtree(M_LEVEL + 1, childPosition, childWidth, childHeight, mObjects)); // Top left

    childPosition.x += childWidth;
    mChildren.push_back(Quadtree(M_LEVEL + 1, childPosition, childWidth, childHeight, mObjects)); // Top right

    childPosition.z += childWidth;
    mChildren.push_back(Quadtree(M_LEVEL + 1, childPosition, childWidth, childHeight, mObjects)); // Bottom right

    childPosition.x -= childWidth;
    mChildren.push_back(Quadtree(M_LEVEL + 1, childPosition, childWidth, childHeight, mObjects)); // Bottom left

}

bool Quadtree::isPointInBounds(glm::vec3 p) const
{
    if(p.x < M_POSITION.x)
        return false;

    if(p.x > M_POSITION.x + M_WIDTH)
        return false;

    if(p.z < M_POSITION.z)
        return false;

    if(p.z > M_POSITION.z + M_HEIGHT)
        return false;

    return true;
}


bool rayIntersectsAABB(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 topLeft, float width, float height, float& t)
{
    float raySlope = rayDirection.z / rayDirection.x;

    glm::vec3 d = topLeft - rayOrigin;
    float z = d.x * raySlope;
    float tZ1 = d.x * d.x + z * z;
    if(d.x * rayDirection.x < 0.f)
        tZ1 = -tZ1;


    d.x += width;
    z = d.x * raySlope;
    float tZ2 = d.x * d.x + z * z;
    if(d.x * rayDirection.x < 0.f)
        tZ2 = -tZ2;

    raySlope = 1 / raySlope;

    float x = d.z * raySlope;
    float tX1 = x * x + d.z * d.z;
    if(d.z * rayDirection.z < 0.f)
        tX1 = -tX1;

    d.z += height;
    x = d.z * raySlope;
    float tX2 = x * x + d.z * d.z;
    if(d.z * rayDirection.z < 0.f)
        tX2 = -tX2;


    float tZMin;
    float tZMax;
    if(tZ1 < tZ2)
    {
        tZMin = tZ1;
        tZMax = tZ2;
    }
    else
    {
        tZMin = tZ2;
        tZMax = tZ1;
    }

    float tXMin;
    float tXMax;
    if(tX1 < tX2)
    {
        tXMin = tX1;
        tXMax = tX2;
    }
    else
    {
        tXMin = tX2;
        tXMax = tX1;
    }

    float tMin = tXMin > tZMin ? tXMin : tZMin;
    float tMax = tXMax < tZMax ? tXMax : tZMax;

    if(tMax < 0.f || tMin > tMax)
        return false;


    t = tMin > 0.f ? tMin : tMax;
    return true;
}

bool Quadtree::isOutOfBounds(glm::vec3 eye, glm::vec3 leftEdge, glm::vec3 rightEdge, float farDistanceSqrd) const
{

    if(isPointInBounds(eye))
        return false;

    glm::vec3 d;
    float t1;
    float t2;
    float t3;
    float t4;


//    glm::vec3 tZ1 = (M_POSITION.x - eye.x) * leftEdge.x * leftEdge;
//    glm::vec3 tZ2 = (M_POSITION.x + M_WIDTH - eye.x) * leftEdge.x * leftEdge;
//
//    glm::vec3 tX1 = (M_POSITION.z - eye.z) * leftEdge.z * leftEdge;
//    glm::vec3 tX1 = (M_POSITION.z + M_HEIGHT - eye.z) * leftEdge.z * leftEdge;
//
    float t = 0.f;
    if(rayIntersectsAABB(eye, leftEdge, M_POSITION, M_WIDTH, M_HEIGHT, t))
    {
        if(t < farDistanceSqrd)
            return false;
//        if(t * t < farDistanceSqrd)
//            return false;
    }

    if(rayIntersectsAABB(eye, rightEdge, M_POSITION, M_WIDTH, M_HEIGHT, t))
    {
        if(t < farDistanceSqrd)
            return false;
    }

    d = M_POSITION - eye;
    d.y = 0.f;
    // Check if any point is within radius.
    t1 = glm::dot(d, d);
    if(t1 > farDistanceSqrd)
    {
        d.x += M_WIDTH;
        t1 = glm::dot(d, d);

        if(t1 > farDistanceSqrd)
        {
            d.z += M_HEIGHT;
            t1 = glm::dot(d, d);

            if(t1 > farDistanceSqrd)
            {
                d.x -= M_WIDTH;
                t1 = glm::dot(d, d);

                if(t1 > farDistanceSqrd)
                    return true;
            }
        }
    }

    // Check if any point is between frustum edges.
    glm::vec3 nLeft(-leftEdge.z, 0.f, leftEdge.x);
    glm::vec3 nRight(rightEdge.z, 0.f, -rightEdge.x);

    d = M_POSITION - eye;

    t1 = glm::dot(nLeft, d);
    if(t1 > 0.f)
    {
        t1 = glm::dot(nRight, d);
        if(t1 > 0.f)
            return false;
    }

    d.x += M_WIDTH;
    t1 = glm::dot(nLeft, d);
    if(t1 > 0.f)
    {
        t1 = glm::dot(nRight, d);
        if(t1 > 0.f)
            return false;
    }

    d.z += M_HEIGHT;
    t1 = glm::dot(nLeft, d);
    if(t1 > 0.f)
    {
        t1 = glm::dot(nRight, d);
        if(t1 > 0.f)
            return false;
    }

    d.x -= M_WIDTH;
    t1 = glm::dot(nLeft, d);
    if(t1 > 0.f)
    {
        t1 = glm::dot(nRight, d);
        if(t1 > 0.f)
            return false;
    }

    return true;


/*
    float tZ1 = (M_POSITION.x - eye.x) * leftEdge.x;
    float tZ2 = (M_POSITION.x + M_WIDTH - eye.x) * leftEdge.x;

    float tX1 = (M_POSITION.z - eye.z) * leftEdge.z;
    float tX2 = (M_POSITION.z + M_HEIGHT - eye.z) * leftEdge.z;

    float tZMin;
    float tZMax;
    if(tZ1 < tZ2)
    {
        tZMin = tZ1;
        tZMax = tZ2;
    }
    else
    {
        tZMin = tZ2;
        tZMax = tZ1;
    }

    float tXMin;
    float tXMax;
    if(tX1 < tX2)
    {
        tXMin = tX1;
        tXMax = tX2;
    }
    else
    {
        tXMin = tX2;
        tXMax = tX1;
    }

    float tMin = tXMin > tZMin ? tXMin : tZMin;
    float tMax = tXMax < tZMax ? tXMax : tZMax;




    if(tMax > 0.f && tMin < tMax && tMin * tMin < farDistanceSqrd)
        return false;
*/
    return true;





        /////////









/*

    if(t1 < 0.f && t2 < 0.f && t3 < 0.f && t4 < 0.f)
        return true;

    d = M_POSITION - eye;


    t1 = glm::dot(n, d);

    d.x += M_WIDTH;
    t2 = glm::dot(n, d);

    d.z += M_HEIGHT;
    t3 = glm::dot(n, d);

    d.x -= M_WIDTH;
    t4 = glm::dot(n, d);

    if(t1 < 0.f && t2 < 0.f && t3 < 0.f && t4 < 0.f)
        return true;



*/

    return false;
}


bool Quadtree::isOutOfBounds(glm::vec3 position, float radius) const
{
    // First check if quad does not intersect the circle's bounding rectangle.
    glm::vec3 edge = position;
    edge.z += radius;
    if(edge.z < M_POSITION.z) // Is rectangle's bottom farther than quad's top?
        return true;

    edge.x += radius;
    if(edge.x < M_POSITION.x) // Is rectangle's right to the left of quad's left?
        return true;

    edge.z -= 2.f * radius;
    if(edge.z > M_POSITION.z + M_HEIGHT) // Is rectangle's top closer than quad's bottom?
        return true;

    edge.x -= 2.f * radius;
    if(edge.x > M_POSITION.x + M_WIDTH) // Is rectangle's left to the right of quad's right?
        return true;

    if(isPointInBounds(position))
        return false;

    // Check if any of this quad's vertices are inside the bounding circle.
    float radiusSqrd = radius * radius;
    glm::vec3 distanceVector;

    distanceVector = M_POSITION - position;
    distanceVector.y = 0.f;
    if(radiusSqrd > glm::dot(distanceVector, distanceVector))
        return false;

    distanceVector.x += M_WIDTH;
    if(radiusSqrd > glm::dot(distanceVector, distanceVector))
        return false;

    distanceVector.z += M_HEIGHT;
    if(radiusSqrd > glm::dot(distanceVector, distanceVector))
        return false;

    distanceVector.x -= M_WIDTH;
    if(radiusSqrd > glm::dot(distanceVector, distanceVector))
        return false;


    // Only possible intersection remaining is a quad edge intersecting
    // the circle with both points still outside the circle.
    if(M_POSITION.x < position.x && M_POSITION.x + M_WIDTH > position.x)
        return false;

    if(M_POSITION.z < position.z && M_POSITION.z + M_HEIGHT > position.z)
        return false;


    return true;

/*
 * This check is for a goddamn rectangle. Oops
    position.z += radius;
    if(position.z < M_POSITION.z) // Is object's bottom farther than quad's top?
        return true;

    position.x += radius;
    if(position.x < M_POSITION.x) // Is object's right to the left of quad's left?
        return true;

    position.z -= 2.f * radius;
    if(position.z > M_POSITION.z + M_HEIGHT) // Is object's top closer than quad's bottom?
        return true;

    position.x -= 2.f * radius;
    if(position.x > M_POSITION.x + M_WIDTH) // Is object's left to the right of quad's right?
        return true;
*/
    return false;
}

//
//std::list<Quadtree::Quad> Quadtree::getIntersectingQuads(glm::vec3 position, float radius) const
//{
//    std::list<Quad> quads;
//    getIntersectingQuads(position, radius, quads);
//
//    return quads;
//}


//void Quadtree::getIntersectingQuads(glm::vec3 position, float radius, std::list<Quad>& quads) const
//{
//    if(isOutOfBounds(position, radius))
//        return;
//
//    if(mChildren.empty())
//    {
//        Quad quad;
//        quad.height = M_HEIGHT;
//        quad.position = M_POSITION;
//        quad.width = M_WIDTH;
//        quads.push_back(quad);
//    }
//    else
//    {
//        for(const Quadtree& child : mChildren)
//            child.getIntersectingQuads(position, radius, quads);
//    }
//}
//#include <iostream>
//std::list<Quadtree::Quad> Quadtree::getIntersectingQuads(glm::vec3 eye, glm::vec3 leftEdge, glm::vec3 rightEdge, float farDistanceSqrd) const
//{
//    std::list<Quad> quads;
////
////    leftEdge.y = 0.f;
////    rightEdge.y = 0.f;
////
////    leftEdge = glm::normalize(leftEdge);
////    rightEdge = glm::normalize(rightEdge);
//
//    getIntersectingQuads(eye, leftEdge, rightEdge, farDistanceSqrd, quads);
//
//    return quads;
//}


//void Quadtree::getIntersectingQuads(glm::vec3 eye, glm::vec3 leftEdge, glm::vec3 rightEdge, float farDistanceSqrd, std::list<Quad>& quads) const
//{
//    if(isOutOfBounds(eye, leftEdge, rightEdge, farDistanceSqrd))
//        return;
//
//    if(mChildren.empty())
//    {
//        Quad quad;
//        quad.height = M_HEIGHT;
//        quad.position = M_POSITION;
//        quad.width = M_WIDTH;
//        quads.push_back(quad);
//    }
//    else
//    {
//        for(const Quadtree& child : mChildren)
//            child.getIntersectingQuads(eye, leftEdge, rightEdge, farDistanceSqrd, quads);
//    }
//}
//
//



void Quadtree::getQuads(std::list<Quad>& quads) const
{
    quads.push_back({M_POSITION, M_WIDTH, M_HEIGHT});
    for(const Quadtree& child : mChildren)
        child.getQuads(quads);
}

//
//void Quadtree::getIntersectingQuads(glm::vec3 topLeft, glm::vec3 right, glm::vec3 down, std::list<Quad>& quads) const
//{
//    if(isOutOfBounds(topLeft, right, down))
//        return;
//
//    if(mChildren.empty())
//    {
//        Quad quad;
//        quad.height = M_HEIGHT;
//        quad.position = M_POSITION;
//        quad.width = M_WIDTH;
//        quads.push_back(quad);
//    }
//    else
//    {
//        for(const Quadtree& child : mChildren)
//            child.getIntersectingQuads(topLeft, right, down, quads);
//    }
//}

//std::list<Quadtree::Quad> Quadtree::getIntersectingQuads(glm::vec3 topLeft, glm::vec3 right, glm::vec3 down) const
//{
//    std::list<Quad> quads;
//    getIntersectingQuads(topLeft, right, down, quads);
//
//    return quads;
//}

bool isPointInRectYo(glm::vec3 p, glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 botLeft)
{
    if(glm::dot(topLeft - botLeft, topLeft - botLeft) == 0.f)
        return false;

    if(glm::dot(topLeft - topRight, topLeft - topRight) == 0.f)
        return false;

    if(glm::dot(p - topLeft, topRight - topLeft) < 0.f)
        return false;

    if(glm::dot(p - topRight, topLeft - topRight) < 0.f)
        return false;

    if(glm::dot(p - topLeft, botLeft - topLeft) < 0.f)
        return false;

    if(glm::dot(p - botLeft, topLeft - botLeft) < 0.f)
        return false;

    return true;
}

bool Quadtree::isOutOfBounds(glm::vec3 topLeft, glm::vec3 right, glm::vec3 down) const
{
    if(isPointInBounds(topLeft))
        return false;

    if(isPointInRectYo(M_POSITION, topLeft, topLeft + right, topLeft + down))
        return false;

    float t;
    float widthSqrd = glm::dot(right, right);
    float heightSqrd = glm::dot(down, down);
    if(rayIntersectsAABB(topLeft, right, M_POSITION, M_WIDTH, M_HEIGHT, t))
        if(t < widthSqrd)
            return false;

    if(rayIntersectsAABB(topLeft, down, M_POSITION, M_WIDTH, M_HEIGHT, t))
        if(t < heightSqrd)
            return false;

    glm::vec3 botRight = topLeft + right + down;
    if(rayIntersectsAABB(botRight, -right, M_POSITION, M_WIDTH, M_HEIGHT, t))
        if(t < widthSqrd)
            return false;

    if(rayIntersectsAABB(botRight, -down, M_POSITION, M_WIDTH, M_HEIGHT, t))
        if(t < heightSqrd)
            return false;

    return true;
}
