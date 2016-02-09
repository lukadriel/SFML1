#pragma once
#include <SFML/Graphics.hpp>
#include <tmx/MapLoader.h>

namespace Textures
{
	enum ID
	{
		Main,
		Enemy,
		Allied,
		Bullet,
		Missile,
		FireRate,
		FireSpread,
		MissileRefill,
		HealthRefill,
		Ground,
		TitleScreen,
		ButtonNormal,
		ButtonSelected,
		ButtonPressed,
		Tileset,
		Wall,
		End,
	};
}

namespace Fonts
{
	enum ID
	{
		Main,
	};
}

namespace Map
{
	enum ID
	{
		Level1,
	};
}


template <typename Resource, typename Identifier>
class ResourceHolder;
typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder <sf::Font, Fonts::ID> FontHolder;
typedef ResourceHolder <tmx::MapLoader, Map::ID> MapHolder;