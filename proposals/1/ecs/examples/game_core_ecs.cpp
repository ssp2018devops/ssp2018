
#include <ecs/ecs.hpp>

#include <functional>
#include <memory>

struct UserInput
{
    // user input stuff ...
};

struct TimeStep
{
    float dt;
};

struct SystemFunction
{
    std::function<void(ecs::World&)> function;
};

class System;

struct SystemPointer
{
    std::unique_ptr<System> system;
};

class GameCore
{
    public:
        GameCore();

        void register_function(const SystemFunction& function); // one alternative
        void register_system(SystemPointer&& system); // another alternative
        void update();

    private:
        ecs::World _world;
        ecs::Entity _root;
};

class System
{
    public:
        virtual ~System();

        virtual void initialize(ecs::World& world);
        virtual void update(ecs::World& world);
};


GameCore::GameCore()
: _world()
, _root(_world.create_entity())
{
}

void GameCore::register_function(const SystemFunction& function)
{
    _root += function;
}

void GameCore::register_system(SystemPointer&& system)
{
    if(system.system)
    {
        System* s = system.system.get();
        _root.attach_move(std::move(system));
        s->initialize(_world);
    }
}

void GameCore::update()
{
    _root.detach<UserInput>();
    _root += UserInput();
    
    _root.detach<TimeStep>();
    _root += TimeStep();


    for(SystemFunction* function : _world.get_components<SystemFunction>())
    {
        function->function(_world);
    }

    for(SystemPointer* system : _world.get_components<SystemPointer>())
    {
        system->system->update(_world);
    }
}


System::~System()
{

}

void System::initialize(ecs::World& world)
{

}

void System::update(ecs::World& world)
{

}








void physics_stuff(ecs::World& world)
{
    // ...
}

class CollisionStuff
{
    public:
        static void plug_into_core(GameCore& core);

    private:
        void update(ecs::World& world);

        // lots of state data
};

void CollisionStuff::plug_into_core(GameCore& core)
{
    std::shared_ptr<CollisionStuff> stuff(new CollisionStuff());
    core.register_function({[stuff](ecs::World& world)
    {
        stuff->update(world);
    }});
}

void CollisionStuff::update(ecs::World& world)
{
    // ...
}



class GuiSystem : public System
{
    public:
        void initialize(ecs::World& world) override;
        void update(ecs::World& world) override;
};

void GuiSystem::initialize(ecs::World& world)
{
    // initialize gui...
}

void GuiSystem::update(ecs::World& world)
{
    // update gui ... handle events...
}


int main()
{
    GameCore core;
    core.register_function({physics_stuff});
    CollisionStuff::plug_into_core(core);
    core.register_system({std::unique_ptr<System>(new GuiSystem())});

    core.update();
    


    return 0;
}

