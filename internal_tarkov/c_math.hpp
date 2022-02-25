#pragma once

class unity::c_math {
public:
	static auto abs( float f ) -> float {
		MONO_METHOD( abs_fn, E( "UnityEngine::Mathf.Abs()" ), 1, float( * )( float ) );
		return abs_fn( f );
	}

	static auto sin( float f ) -> float {
		MONO_METHOD( sin_fn, E( "UnityEngine::Mathf.Sin()" ), 1, float( * )( float ) );
		return sin_fn( f );
	}

	static auto cos( float f ) -> float {
		MONO_METHOD( cos_fn, E( "UnityEngine::Mathf.Cos()" ), 1, float( * )( float ) );
		return cos_fn( f );
	}

	static auto asin( float f ) -> float {
		MONO_METHOD( asin_fn, E( "UnityEngine::Mathf.Asin()" ), 1, float( * )( float ) );
		return asin_fn( f );
	}

	static auto atan2( float f, float x ) -> float {
		MONO_METHOD( atan2_fn, E( "UnityEngine::Mathf.Atan2()" ), 2, float( * )( float, float ) );
		return atan2_fn( f, x );
	}
};