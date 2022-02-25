#pragma once
#include "sdk.hpp"

namespace features 
{
	class c_misc 
	{
	public:
		auto flyhack( ) -> void;
		auto no_visor( ) -> void;
		auto no_stamina( ) -> void;
		auto thermal_vision( ) -> void;
		auto night_vision( ) -> void;
		auto max_search_stat( ) -> void;
		auto speedhack( ) -> void;
	};

	inline c_misc misc;
}