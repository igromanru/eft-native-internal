#pragma once
#include "script_manager.hpp"
#include "js_utils.hpp"

namespace scripting 
{
	class fn {
	public:
		static duk_ret_t draw_rect( duk_context* context ) {

			auto x = static_cast< float >( duk_to_int( context, 0 ) );
			auto y = static_cast< float >( duk_to_int( context, 1 ) );
			auto w = static_cast< float >( duk_to_int( context, 2 ) );
			auto h = static_cast< float >( duk_to_int( context, 3 ) );

			auto color = scripting::util::prop_to_color( context, 4 );

			if ( engine::c_events::get_current_event( )->type( ) == engine::event_type::repaint )
				engine::gl::rect( x, y, w, h, color );

			return 1;
		}

		static duk_ret_t draw_filled_rect( duk_context* context ) {

			auto x = static_cast< float >( duk_to_int( context, 0 ) );
			auto y = static_cast< float >( duk_to_int( context, 1 ) );
			auto w = static_cast< float >( duk_to_int( context, 2 ) );
			auto h = static_cast< float >( duk_to_int( context, 3 ) );

			auto color = scripting::util::prop_to_color( context, 4 );

			if ( engine::c_events::get_current_event( )->type( ) == engine::event_type::repaint )
				engine::gui::filled_rect( { x , y, w, h }, color );

			return 1;
		}

		static duk_ret_t draw_gradient_rect( duk_context* context ) {

			auto x = static_cast< float >( duk_to_int( context, 0 ) );
			auto y = static_cast< float >( duk_to_int( context, 1 ) );
			auto w = static_cast< float >( duk_to_int( context, 2 ) );
			auto h = static_cast< float >( duk_to_int( context, 3 ) );

			auto color = scripting::util::prop_to_color( context, 4 );
			auto color_two = scripting::util::prop_to_color( context, 5 );

			if ( engine::c_events::get_current_event( )->type( ) == engine::event_type::repaint )
				engine::gl::gradient_filled_rect( x, y, w, h, color, color_two );

			return 1;
		}

		static duk_ret_t calculate_text_size( duk_context* context ) {

			auto text = std::string( duk_to_string( context, 0 ) );
			auto wide = std::wstring( text.begin( ), text.end( ) );

			auto size = engine::gui::calculate_text_size( wide.c_str( ) );
			auto duk_array = duk_push_array( context );
			
			duk_push_number( context, size.x );
			duk_put_prop_index( context, duk_array, 0 );

			duk_push_number( context, size.y );
			duk_put_prop_index( context, duk_array, 1 );

			return 1;
		}

		static duk_ret_t get_screen_metrics( duk_context* context ) {

			auto w = unity::c_screen::get_width( );
			auto h = unity::c_screen::get_height( );

			auto duk_array = duk_push_array( context );

			duk_push_number( context, w );
			duk_put_prop_index( context, duk_array, 0 );

			duk_push_number( context, h );
			duk_put_prop_index( context, duk_array, 1 );

			return 1;
		}

		static duk_ret_t draw_circle( duk_context* context ) {

			auto x = static_cast< float >( duk_to_int( context, 0 ) );
			auto y = static_cast< float >( duk_to_int( context, 1 ) );

			auto radius = static_cast< float >( duk_to_int( context, 2 ) );

			auto color = scripting::util::prop_to_color( context, 3 );

			if ( engine::c_events::get_current_event( )->type( ) == engine::event_type::repaint )
				engine::gl::circle( x, y, radius, color );

			return 1;
		}

		static duk_ret_t draw_line( duk_context* context ) {

			auto p1 = scripting::util::prop_to_vec2( context, 0 );
			auto p2 = scripting::util::prop_to_vec2( context, 1 );

			auto color = scripting::util::prop_to_color( context, 2 );

			if ( engine::c_events::get_current_event( )->type( ) == engine::event_type::repaint )
				engine::gl::line( p1.x, p1.y, p2.x, p2.y, color );

			return 1;
		}

		static duk_ret_t draw_string( duk_context* context ) {

			auto x = static_cast< float >( duk_to_int( context, 0 ) );
			auto y = static_cast< float >( duk_to_int( context, 1 ) );

			auto text = std::string( duk_to_string( context, 2 ) );
			auto wide = std::wstring( text.begin( ), text.end( ) );

			auto color = scripting::util::prop_to_color( context, 3 );

			if ( engine::c_events::get_current_event( )->type( ) == engine::event_type::repaint )
				engine::gui::string( x, y, 600, 20, wide.c_str( ), color, false, false );

			return 1;
		}
	};
}