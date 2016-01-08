#pragma once
#include "Entity.h"
//#include "ResourceIdentifier.h"
#include "ResourceHolder.h"

class Character: public Entity
{
public:
	enum Type
	{
		Main,
		Enemy,
	};
	explicit Character(Type type,const TextureHolder& textures);	
	virtual unsigned int getCategory();
private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	Type mType;
	sf::Sprite mSprite;
};

