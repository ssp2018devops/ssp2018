
#include <ecs/ecs.hpp>

#include <functional>
#include <memory>

struct UserInput
{
    // user input stuff ...
};

class System;
class GameCore
{
    public:
        using Function = std::function<void(ecs::World& world, const UserInput& input, float dt)>;

        void register_function(const Function& function); // one alternative
        void register_system(std::unique_ptr<System>&& system); // another alternative
        void update();

    private:
        ecs::World _world;
        std::vector<Function> _functions;
        std::vector<std::unique_ptr<System>> _systems;
};

class System
{
    public:
        virtual ~System();

        virtual void initialize(ecs::World& world);
        virtual void update(ecs::World& world, const UserInput& input, float dt);
};


void GameCore::register_function(const Function& function)
{
    _functions.push_back(function);
}

void GameCore::register_system(std::unique_ptr<System>&& system)
{
    if(system)
    {
        _systems.emplace_back(std::move(system));
    }
}

void GameCore::update()
{
    UserInput input;
    static constexpr float step = 1.f;
    for(const Function& function : _functions)
    {
        function(_world, input, step);
    }

    for(const std::unique_ptr<System>& system : _systems)
    {
        system->update(_world, input, step);
    }
}


System::~System()
{

}

void System::initialize(ecs::World& world)
{

}

void System::update(ecs::World& world, const UserInput& input, float dt)
{

}








void physics_stuff(ecs::World& world, const UserInput& input, float dt)
{
    // ...
}

class CollisionStuff
{
    public:
        static void plug_into_core(GameCore& core);

    private:
        void update(ecs::World& world, const UserInput& input, float dt);

        // lots of state data
};

void CollisionStuff::plug_into_core(GameCore& core)
{
    std::shared_ptr<CollisionStuff> stuff(new CollisionStuff());
    core.register_function([stuff](ecs::World& world, const UserInput& input, float dt)
    {
        stuff->update(world, input, dt);
    });
}

void CollisionStuff::update(ecs::World& world, const UserInput& input, float dt)
{
    // ...
}



class GuiSystem : public System
{
    public:
        void initialize(ecs::World& world) override;
        void update(ecs::World& world, const UserInput& input, float dt) override;
};

void GuiSystem::initialize(ecs::World& world)
{
    // initialize gui...
}

void GuiSystem::update(ecs::World& world, const UserInput& input, float dt)
{
    // update gui ... handle events...
}


int main()
{
    GameCore core;
    core.register_function(physics_stuff);
    CollisionStuff::plug_into_core(core);
    core.register_system(std::unique_ptr<System>(new GuiSystem()));

    core.update();
    


    return 0;
}

