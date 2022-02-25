#pragma once
#include "script_manager.hpp"
#include "js_utils.hpp"

namespace scripting 
{
	class js_engine 
	{
	public:
		static duk_ret_t world_to_screen( duk_context* context )
		{
			auto position = scripting::util::prop_to_vec3( context, 0 );

			auto w2s_position = unity::c_camera::get_main( )->world_to_screen_point( position );

			auto duk_array = duk_push_array( context );

			duk_push_number( context, w2s_position.x );
			duk_put_prop_index( context, duk_array, 0 );

			duk_push_number( context, w2s_position.y );
			duk_put_prop_index( context, duk_array, 1 );

			duk_push_number( context, w2s_position.y );
			duk_put_prop_index( context, duk_array, 2 );

			return 1;
		}

		static duk_ret_t get_time( duk_context* context ) 
		{
			duk_push_number( context, unity::c_time::get_time( ) );
			return 1;
		}

		static duk_ret_t get_delta_time( duk_context* context )
		{
			duk_push_number( context, unity::c_time::get_delta_time( ) );
			return 1;
		}
	};

}