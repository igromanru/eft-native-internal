#pragma once

class unity::c_screen {
public:
	static auto get_width( ) -> int {
		MONO_METHOD( get_screen_width_fn, E( "UnityEngine::Screen.get_width()" ), 0, int ( * )( ) );
		return get_screen_width_fn( );
	}

	static auto get_height( ) -> int {
		MONO_METHOD( get_screen_height_fn, E( "UnityEngine::Screen.get_height()" ), 0, int ( * )( ) );
		return get_screen_height_fn( );
	}
};