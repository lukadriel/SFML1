#include "Character.h"

Textures::ID toTextureId(Character::Type type)
{
	switch (type)
	{
	case Character::Main:
		return Textures::Main;
		break;
	case Character::Enemy:
		return Textures::Enemy;
		break;

	}
}

Character::Character(Type type, const TextureHolder & textures):mType(type),
mSprite(textures.get(toTextureId(type)))
{
	mSprite.setTextureRect(sf::IntRect(0, 144, 32, 48));
	sf::FloatRect bounds(mSprite.getLocalBounds());
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Character::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

unsigned int Character::getCategory()
{
	switch (mType)
	{
	case Character::Main:
		return Category::PlayerCharacter;
		break;
	case Character::Enemy:
		Category::EnemyCharacter;
		break;
	default:
		break;
	}
}



