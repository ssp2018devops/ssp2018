#include "gal.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Shape.hpp>

#include <vector>
#include <functional>

namespace gal
{

std::vector<std::function<void()>> _operations;


void addOperation(const std::function<void()>& operation)
{
    _operations.push_back(operation);
}


void setString(sf::Text& text, const sf::String& string)
{
    addOperation([&text, string](){text.setString(string);});
}

void setPosition(sf::Shape& shape, float x, float y)
{
    addOperation([&shape, x, y](){shape.setPosition(x, y);});
}

void setFillColor(sf::Shape& shape, const sf::Color& color)
{
    addOperation([&shape, color](){shape.setFillColor(color);});
}


void commit()
{
    for(const std::function<void()>& operation : _operations)
    {
        operation();
    }
    _operations.clear();
}



}
