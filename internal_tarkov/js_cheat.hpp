#pragma once
#include "script_manager.hpp"
#include <algorithm>

namespace scripting
{
	enum js_callbacks : int {
		PAINT,
		UPDATE
	};

	class cheat
	{
	public:
		static duk_ret_t push_notification( duk_context* context ) {

			auto string = std::string( duk_to_string( context, 0 ) );

			features::notifcations.push( std::wstring( string.begin( ), string.end( ) ).c_str( ), unity::c_time::get_time( ) );

			return 1;
		}

		static void is_duplicated_callback( std::vector< std::string > vec, std::string what ) {
			const auto duplicate = std::find( vec.begin( ), vec.end( ), what );
			if ( duplicate != vec.end( ) )
				features::notifcations.push( E( L"callback defined with same name in another script!" ), unity::c_time::get_time( ) );
		}

		static duk_ret_t add_callback( duk_context* context )
		{
			auto callback = duk_to_int( context, 0 );
			auto fn_callback = duk_to_string( context, 1 );

			switch ( callback )
			{
			case js_callbacks::PAINT:
				scripting::object.paint_callbacks.push_back( fn_callback );
				is_duplicated_callback( scripting::object.paint_callbacks, fn_callback );
				return 0;
			case js_callbacks::UPDATE:
				scripting::object.update_callbacks.push_back( fn_callback );
				is_duplicated_callback( scripting::object.update_callbacks, fn_callback );
				return 0;
			}

			return 1;
		}
	};
}