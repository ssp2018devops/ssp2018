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
    update_gui_selection(world); // prints nothing
    select(button_a);
    world.swap_events();

    // Tick 2
    update_gui_selection(world); // "selected 'a'"
    select(button_b);
    world.swap_events();

    // Tick 3
    update_gui_selection(world); // "deselected 'a', selected 'b'""
    world.swap_events();

    return 0;
}