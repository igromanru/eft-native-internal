#pragma once
#include "sdk.hpp"

namespace features 
{
	class c_weapon_mods 
	{
	public:
		auto weapon_removals( ) -> void;
		auto instant_aim( ) -> void;
	};

	inline c_weapon_mods weapon;
}