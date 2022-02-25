#pragma once
#include "js_utils.hpp"

namespace scripting {

	class entity_list {
	public:
		static duk_ret_t get_player( duk_context* context ) { }
		static duk_ret_t get_player_count( duk_context* context ) { }
		static duk_ret_t get_local_player( duk_context* context ) {
		
			//duk_push_int( context, 0 );

			return 1;
		}
		static duk_ret_t get_player( duk_context* context ) { }
	};

	class player {
	public:
		static duk_ret_t get_bone_position( duk_context* context ) 
		{
			//auto bone_index = duk_to_int

			return 1;
		}
	};
}