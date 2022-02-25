#pragma once

class unity::c_time 
{
public:
	static auto get_time( ) -> float 
	{
		MONO_METHOD( get_time_fn, E( "UnityEngine::Time.get_time()" ), 0, float( * )( ) );
		return get_time_fn( );
	}

	static auto get_delta_time( ) -> float 
	{
		MONO_METHOD( get_delta_time_fn, E( "UnityEngine::Time.get_deltaTime()" ), 0, float( * )( ) );
		return get_delta_time_fn( );
	}

	static auto set_timescale( float scale ) -> void
	{
		MONO_METHOD( set_timescale_fn, E( "UnityEngine::Time.set_timeScale()" ), 1, void( * )( float ) );
		return set_timescale_fn( scale );
	}
};