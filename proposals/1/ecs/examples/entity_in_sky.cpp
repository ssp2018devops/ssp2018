
#include <ecs/ecs.hpp>

#include <cassert>

struct HasGravity
{
};

struct Altitude
{
    float height;
};

struct Health
{
    unsigned short amount;
};


ecs::Entity spawn_entity_in_sky(ecs::World& world)
{
    // Create the entity.
    ecs::Entity entity = world.create_entity();

    // Implicitly attach an Altitude component
    // to the entity and get it.
    Altitude& altitude = entity;
    altitude.height = 10.f;

    // Explicitly attach a Health component
    // and write to it.
    entity.attach<Health>().amount = 100;

    // Explicitly attach a HasGravity component.
    entity += HasGravity();

    return entity;
}

void update_gravity(ecs::World& world, float dt)
{
    // Create a key that matches all entities with HasGravity
    // component.
    ecs::Key gravity_key;
    gravity_key.include<HasGravity>();

    // Update all altitude components that are attached
    // to entities with a HasGravity component.
    for(Altitude* altitude : world.get_components<Altitude>(gravity_key))
    {
        altitude->height -= 100.f * dt;
    }
}

void update_health(ecs::World& world)
{
    // Create a key that matches all entities with HasGravity
    // component.
    ecs::Key gravity_key;
    gravity_key.include<HasGravity>();

    // Update healths through tuples.
    for(std::tuple<Health*, Altitude*> tuple : world.get_component_tuples<Health, Altitude>(gravity_key))
    {
        if(std::get<Altitude*>(tuple)->height < 0.f)
        {
            std::get<Health*>(tuple)->amount = 0;
        }
    }


    // Or update healths through entities.
    ecs::Key key;
    key.include<Health, Altitude, HasGravity>();

    for(ecs::Entity entity : world.get_entities(key))
    {
        // Get existing altitude.
        Altitude* altitude = entity;

        // We already know that this entity has an altitude since it
        // got matched with the key. But let's check anyway.
        assert(altitude);

        // Let's check the other two components while we're at it.
        assert(entity.has<Health>());
        assert(entity.has<HasGravity>());

        if(altitude->height < 0.f)
        {
            entity.get<Health>()->amount = 0;
        }
    }
}

void update_death(ecs::World& world)
{
    // Get all entities with a Health component.
    ecs::Key key;
    key.include<Health>();
    
    for(ecs::Entity entity : world.get_entities(key))
    {
        // Kill it if it has zero health.
        if(entity.get<Health>()->amount == 0)
        {
            world.destroy_entity(entity);
        }
    }
}

int main()
{
    // Create world.
    ecs::World world;

    // Spawn entity in sky.
    ecs::Entity entity_in_sky = spawn_entity_in_sky(world);

    // Invoke gravity on the world a couple of times.
    for(size_t i = 0; i < 10; i++)
    {
        update_gravity(world, 1.f);
    }

    // Update the health of the world's entities.
    update_health(world);

    // Tell Death to visit all unfortunate souls.
    update_death(world);

    // What if our entity in the sky died?
    // Let's check.
    if(!entity_in_sky)
    {
        // Oh no...
    }

    return 0;
}