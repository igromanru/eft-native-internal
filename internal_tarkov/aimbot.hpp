#pragma once
#include "sdk.hpp"

namespace features 
{
	class c_aimbot 
	{
	public:
		struct shot {
			geometry::vec3_t start, end;
			float time;
			shot( geometry::vec3_t s, geometry::vec3_t e, float t ) : start( s ), end( e ), time( t ) { }
		};

		auto find_aimbot_target( ) -> void;
		auto run_aimbot( ) -> void;
		auto get_aimbone_index( int menu_index ) -> int;
		c_player* aimbot_target;

		std::uintptr_t colliders;

		std::vector<shot> shots;
	};

	inline c_aimbot aimbot;
}