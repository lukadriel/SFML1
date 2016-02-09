#include "DataTables.h"
#include "Character.h"
#include "Projectile.h"
#include "Pickup.h"

using namespace std::placeholders;

std::vector<CharacterData> initializeCharacterData()
{
	std::vector<CharacterData> data(Character::TypeCount);
	data[Character::Main].hitpoints = 100;
	data[Character::Main].speed = 200.f;
	data[Character::Main].texture = Textures::Main;
	data[Character::Main].fireInterval=sf::seconds(1.f);
	
	data[Character::Enemy].hitpoints = 20;
	data[Character::Enemy].speed = 50.f;
	data[Character::Enemy].texture = Textures::Enemy;
	data[Character::Enemy].directions.push_back(Direction(+45.f, 80.f));
	data[Character::Enemy].directions.push_back(Direction(-45.f, 160.f));
	data[Character::Enemy].directions.push_back(Direction(+45.f, 80.f));
	data[Character::Enemy].fireInterval = sf::seconds(3.f);

	return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
	std::vector<ProjectileData> data(Projectile::TypeCount);
	data[Projectile::AlliedBullet].damage = 10;
	data[Projectile::AlliedBullet].speed = 300.f;
	data[Projectile::AlliedBullet].texture = Textures::Bullet;

	data[Projectile::EnemyBullet].damage = 10;
	data[Projectile::EnemyBullet].speed = 300.f;
	data[Projectile::EnemyBullet].texture = Textures::Bullet;

	data[Projectile::Missile].damage = 200;
	data[Projectile::Missile].speed = 150.f;
	data[Projectile::Missile].texture = Textures::Missile;
	return data;
}

std::vector<PickupData> initializePickUpData()
{
	std::vector<PickupData> data(Pickup::TypeCount);

	data[Pickup::HealthRefill].texture = Textures::HealthRefill;
	data[Pickup::HealthRefill].action = [](Character& a) { a.repair(25); };

	/*data[Pickup::MissileRefill].texture = Textures::MissileRefill;
	data[Pickup::MissileRefill].action = std::bind(&Character::collectMissiles, _1, 3);*/

	data[Pickup::FireSpread].texture = Textures::FireSpread;
	data[Pickup::FireSpread].action = std::bind(&Character::increaseSpread, _1);

	data[Pickup::FireRate].texture = Textures::FireRate;
	data[Pickup::FireRate].action = std::bind(&Character::increaseFireRate, _1);

	return data;
}
