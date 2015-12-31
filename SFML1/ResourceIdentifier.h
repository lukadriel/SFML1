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


template <typename Resource, typename Identifier>
class ResourceHolder;
typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;