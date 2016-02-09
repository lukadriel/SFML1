#pragma once

namespace Category
{
	enum Type
	{
		None = 0,
		Scene = 1 << 0,
		PlayerCharacter = 1 << 1,
		AlliedCharacter = 1 << 2,
		EnemyCharacter = 1 << 3,
		Pickup = 1 << 4,
		AlliedProjectile = 1 << 5,
		EnemyProjectile = 1 << 6,
		Wall = 1 << 7,

		Character = PlayerCharacter | AlliedCharacter| EnemyCharacter,
		Projectile = AlliedProjectile | EnemyProjectile,
	};
}
