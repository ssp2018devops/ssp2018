#include <ecs/ecs.hpp>


#include <iostream>

struct GuiElement
{
    std::string name;
};

struct IsSelected
{

};


void update_gui_selection(ecs::World& world)
{
    for(ecs::Entity deselected : world.get_detached_entities<IsSelected>())
    {
        GuiElement* element = deselected;
        if(element)
        {
            std::cout << "deselected '" << element->name << "'" << std::endl; 
        }
        else
        {
            std::cout << "deselected unknown" << std::endl;
        }
    }

    for(ecs::Entity selected : world.get_attached_entities<IsSelected>())
    {
        GuiElement* element = selected;
        if(element)
        {
            std::cout << "selected '" << element->name << "'" << std::endl; 
        }
        else
        {
            std::cout << "selected unknown" << std::endl;
        }
    }
}


void select(ecs::Entity& entity)
{
    entity.get_world().detach_components<IsSelected>();
    entity += IsSelected();
}


int main()
{
    ecs::World world;
    ecs::Entity button_a = world.create_entity();
    GuiElement& element = button_a;
    element.name = "a";

    ecs::Entity button_b = world.create_entity();
    element = button_b;
    element.name = "b";

    // Tick 1
    select(button_a);
    update_gui_selection(world); // selected 'a'
    world.end_tick();

    // Tick 2
    select(button_b);
    update_gui_selection(world); // deselected 'a', selected 'b'
    world.end_tick();

    return 0;
}