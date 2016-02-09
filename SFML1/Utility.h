#pragma once

#include <sstream>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
	class Sprite;
	class Text;
}



void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
std::string toString(sf::Keyboard::Key key);

float toDegree(float radian);
float toRadian(float degree);
float length(sf::Vector2f vector);
sf::Vector2f unitVector(sf::Vector2f vector);
int randomInt(int exclusiveMax);

//#include "Utility.inl"

