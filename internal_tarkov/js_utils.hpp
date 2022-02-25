#pragma once
#include "script_manager.hpp"

namespace scripting {
	
	// these are ugly as fuck but idgaf.
	class util {
	public:
		static geometry::vec3_t prop_to_vec3( duk_context* context, int index ) {
			geometry::vec3_t vec;

			duk_get_prop_index( context, index, 0 );
			vec.x = float( duk_to_number( context, -1 ) );
			duk_pop( context );

			duk_get_prop_index( context, index, 1 );
			vec.y = float( duk_to_number( context, -1 ) );
			duk_pop( context );

			duk_get_prop_index( context, index, 2 );
			vec.z = float( duk_to_number( context, -1 ) );
			duk_pop( context );

			return vec;
		}


		static geometry::vec2_t prop_to_vec2( duk_context* context, int index ) {
			geometry::vec2_t vec;

			duk_get_prop_index( context, index, 0 );
			vec.x = float( duk_to_number( context, -1 ) );
			duk_pop( context );

			duk_get_prop_index( context, index, 1 );
			vec.y = float( duk_to_number( context, -1 ) );
			duk_pop( context );

			return vec;
		}

		static engine::color_t prop_to_color( duk_context* context, int index ) {
			engine::color_t color = engine::color_t( );

			duk_get_prop_index( context, index, 0 );

			color.r = float( duk_to_number( context, -1 ) );
			duk_pop( context );

			duk_get_prop_index( context, index, 1 );
			color.g = float( duk_to_number( context, -1 ) );
			duk_pop( context );

			duk_get_prop_index( context, index, 2 );
			color.b = float( duk_to_number( context, -1 ) );
			duk_pop( context );

			duk_get_prop_index( context, index, 3 );
			color.a = float( duk_to_number( context, -1 ) );
			duk_pop( context );

			return color.normalized( );
		}
	};
}