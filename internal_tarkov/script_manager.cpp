#include "script_manager.hpp"
#include <fstream>
#include "menu.hpp"
#include <filesystem>
auto scripting::c_script_manager::init( ) -> void {
	duk_context = duk_create_heap( 0, 0, 0, 0, js_exception_handler );
}

auto scripting::c_script_manager::push_global( int value, const char* name, const char* prop ) -> void {
	duk_push_object( duk_context );
	duk_push_number( duk_context, value );
	duk_put_prop_string( duk_context, -2, prop );
	duk_put_global_string( duk_context, name );
}

auto scripting::c_script_manager::push_prop_function( duk_c_function function, size_t arguments, const char* name, const char* prop ) -> void {
	duk_push_global_object( duk_context );
	duk_push_c_function( duk_context, function, arguments );
	duk_put_prop_string( duk_context, -2, prop );
	duk_put_global_string( duk_context, name );
}

auto scripting::c_script_manager::push_function( duk_c_function function, size_t arguments, const char* name ) -> void {
	duk_push_c_function( duk_context, function, arguments );
	duk_put_global_string( duk_context, name );
}

auto scripting::c_script_manager::parse_scripts( ) -> void {

	for ( auto& entry : std::filesystem::directory_iterator( "C:\\sapphire\\scripts\\" ) )
	{
		auto file = entry.path( ).string( );

		if ( file.find( ".js" ) == std::string::npos )
			continue;

		if ( std::find( menu::vars::parsed_scripts.begin( ), menu::vars::parsed_scripts.end( ), file ) == menu::vars::parsed_scripts.end( ) )
			menu::vars::parsed_scripts.push_back( file );
	}
}

auto scripting::c_script_manager::parse_file( const char* path ) -> bool {

	std::string file_content;
	std::getline( std::ifstream( path ), file_content, '\0' );

	if ( duk_peval_string( duk_context, file_content.c_str( ) ) != 0 || error_thrown( ) )
	{
		const auto duk_output = std::string( duk_safe_to_string( duk_context, -1 ) );
		const auto notify = std::wstring( duk_output.begin( ), duk_output.end( ) );

		features::notifcations.push( notify.c_str( ), unity::c_time::get_time( ) );

		duk_pop( duk_context );

		return false;
	}

	return true;
}

auto scripting::c_script_manager::js_exception_handler( void* data, const char* exc ) -> void {

	// hardly gets hit but watever..
	const auto str = std::wstring( L"[exc] " ) + std::wstring( std::string( exc ).begin( ), std::string( exc ).end( ) );
	features::notifcations.push( str.c_str( ), unity::c_time::get_time( ) );
	scripting::object.exception = true;
}