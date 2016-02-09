#pragma once
#include "Entity.h"
#include "Command.h"
#include "ResourceIdentifier.h"

#include <SFML/Graphics/Sprite.hpp>

class Character;

class Pickup :public Entity
{
public:
	enum Type
	{
		HealthRefill,
		/*MissileRefill,*/
		FireSpread,
		FireRate,
		TypeCount
	};
public:
	Pickup(Type type,const TextureHolder& textures );
	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;
	void apply(Character& player) const;

protected:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	Type mType;
	sf::Sprite mSprite;
};

