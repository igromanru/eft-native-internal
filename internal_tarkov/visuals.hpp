#pragma once
#include "engine_render.hpp"
#include "sdk.hpp"

namespace features 
{
	class c_visuals 
	{
	public:
		auto render_player( c_player* player ) -> void;
		auto render_extract( std::uintptr_t extract ) -> void;
		auto local_ammo( ) -> void;
	};

	inline c_visuals visuals;
}