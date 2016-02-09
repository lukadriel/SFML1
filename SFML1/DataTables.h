#pragma once

#include "ResourceIdentifier.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <functional>

class Character;

struct Direction
{
	Direction(float angle, float distance) :angle(angle), distance(distance)
	{
	}
	float angle;
	float distance;
};

struct CharacterData
{
	int hitpoints;
	float speed;
	sf::Time fireInterval;
	std::vector<Direction> directions;
	Textures::ID texture;

};



struct ProjectileData
{
	int damage;
	float speed;
	Textures::ID texture;
};

struct PickupData
{
	std::function<void(Character&)> action;
	Textures::ID texture;
};

std::vector<CharacterData> initializeCharacterData();
std::vector<ProjectileData> initializeProjectileData();
std::vector<PickupData> initializePickUpData();