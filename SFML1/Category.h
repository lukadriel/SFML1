#pragma once

namespace Category
{
	enum Type
	{
		None = 0,
		Scene = 1,
		PlayerCharacter = 1 << 1,
		AlliedCharacter = 1 << 2,
		EnemyCharacter = 1 << 3,
	};
}
