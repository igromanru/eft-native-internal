#include "hooks.hpp"
#include "sdk.hpp"

#include "script_manager.hpp"
#include "js_cheat.hpp"
#include "js_renderer.hpp"
#include "js_engine.hpp"

auto impl::hooks::hk_gui_controller_start( void* instance ) -> void
{
	return;
}

static bool wakeup{ true };

auto impl::hooks::hk_gui_controller_update( void* instance ) -> void
{
	if ( wakeup )
	{
		scripting::object.init( );

		scripting::object.parse_scripts( );

		// register cheat functions
		scripting::object.push_global( 0, "callbacks", "PAINT" );
		scripting::object.push_global( 1, "callbacks", "UPDATE" );

		scripting::object.push_prop_function( scripting::cheat::push_notification, 1, "cheat", "add_notification" );
		scripting::object.push_prop_function( scripting::cheat::add_callback, 2, "cheat", "add_callback" );

		scripting::object.push_prop_function( scripting::fn::draw_rect, 5, "renderer", "draw_rect" );
		scripting::object.push_prop_function( scripting::fn::draw_filled_rect, 5, "renderer", "draw_filled_rect" );
		scripting::object.push_prop_function( scripting::fn::draw_string, 4, "renderer", "draw_text" );
		scripting::object.push_prop_function( scripting::fn::draw_line, 3, "renderer", "draw_line" );
		scripting::object.push_prop_function( scripting::fn::draw_circle, 4, "renderer", "draw_circle" );
		scripting::object.push_prop_function( scripting::fn::calculate_text_size, 1, "renderer", "calc_text_size" );
		scripting::object.push_prop_function( scripting::fn::get_screen_metrics, 0, "renderer", "get_screen_metrics" );
		scripting::object.push_prop_function( scripting::fn::draw_gradient_rect, 6, "renderer", "draw_gradient_filled_rect" );

		scripting::object.push_prop_function( scripting::js_engine::world_to_screen, 1, "engine", "world_to_screen" );
		scripting::object.push_prop_function( scripting::js_engine::get_time, 0, "engine", "get_time" );
		scripting::object.push_prop_function( scripting::js_engine::get_delta_time, 0, "engine", "get_delta_time" );


		features::notifcations.push( E( L"all js callbacks setup!" ), unity::c_time::get_time( ) );

		wakeup = false;
	}

	scripting::object.run_update_callbacks( );

	return;
}