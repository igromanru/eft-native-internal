#pragma once
#include "duktape/duktape.h"

#include "notifications.hpp"

namespace scripting
{
	class c_script_manager {
	public:
		auto init( ) -> void;
		auto parse_scripts( ) -> void;
		auto parse_file( const char* path ) -> bool;
		auto push_global( int value, const char* name, const char* prop ) -> void;
		auto push_function( duk_c_function function, size_t arguments, const char* name ) -> void;
		auto push_prop_function( duk_c_function function, size_t arguments, const char* name, const char* prop ) -> void;

		static auto js_exception_handler( void* data, const char* exc ) -> void;

		std::vector < std::string > paint_callbacks;
		std::vector < std::string > update_callbacks;

		duk_context* duk_context;

		auto run_paint_callbacks( ) -> void {
			for ( auto& cb : paint_callbacks ) {
				if ( duk_peval_string( duk_context, ( cb + "();" ).c_str( ) ) != 0 )
				{
					const auto duk_output = std::string( duk_safe_to_string( duk_context, -1 ) );
					const auto notify = std::wstring( duk_output.begin( ), duk_output.end( ) );

					features::notifcations.push( notify.c_str( ), unity::c_time::get_time( ) );
				}
			}
		}

		auto run_update_callbacks( ) -> void {
			for ( auto& cb : update_callbacks ) {
				if ( duk_peval_string( duk_context, ( cb + "();" ).c_str( ) ) != 0 )
				{
					const auto duk_output = std::string( duk_safe_to_string( duk_context, -1 ) );
					const auto notify = std::wstring( duk_output.begin( ), duk_output.end( ) );

					features::notifcations.push( notify.c_str( ), unity::c_time::get_time( ) );
				}
			}
		}

	private:
		auto error_thrown( ) -> bool {
			auto saved = exception;
			exception = false;
			return saved;
		}

		bool exception;
	};

	inline scripting::c_script_manager object;
}