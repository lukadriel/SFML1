#pragma once
#include <SFML/Graphics.hpp>

namespace Textures
{
	enum ID
	{
		Main,
		Enemy,
	};
}

namespace Fonts
{
	enum ID
	{
		Main,
	};
}


template <typename Resource, typename Identifier>
class ResourceHolder;
typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder <sf::Font, Fonts::ID> FontHolder;