#pragma once

#ifndef GAL_HPP
#define GAL_HPP

namespace sf
{
    class Text;
    class String;
    class Color;
    class Shape;
}

namespace gal
{
    void setString(sf::Text& text, const sf::String& string);
    void setPosition(sf::Shape& shape, float x, float y);
    void setFillColor(sf::Shape& shape, const sf::Color& color);


    void commit();
}


#endif // GAL_HPP