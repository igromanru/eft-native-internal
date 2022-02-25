#include "framework.hpp"
#include "menu.hpp"
#include "notifications.hpp"
#include <algorithm>

static bool wakeup{ false };
static bool sex{ false };

bool c_menu_framework::window( const std::string& title, const geometry::vec2_t& pos, const geometry::vec2_t& window_size, bool always_open )
{
	const auto wstring = std::wstring( title.begin( ), title.end( ) );

	if ( !sex ) {
		this->window_pos = pos;
		sex = true;
	}

	this->window_size = window_size;

	MONO_METHOD( get_mouse_pos_fn, E( "UnityEngine::Input.get_mousePosition()" ), -1, geometry::vec3_t( * )( ) );
	MONO_METHOD( get_mouse_button_fn, E( "UnityEngine::Input.GetMouseButton()" ), 1, bool( * )( int ) );
	MONO_METHOD( set_lock_state_fn, E( "UnityEngine::Cursor.set_lockState()" ), -1, void( * )( int ) );
	MONO_METHOD( get_event_keycode_fn, E( "UnityEngine::Event.get_keyCode()" ), -1, engine::key_code( * )( std::uintptr_t ) );
	MONO_METHOD( get_current_event_fn, E( "UnityEngine::Event.get_current()" ), -1, std::uintptr_t( * )( ) );
	MONO_METHOD( set_color_fn, E( "UnityEngine::GUI.set_color()" ), -1, void( * )( engine::color_t ) );
	MONO_METHOD( get_screen_height_fn, E( "UnityEngine::Screen.get_height()" ), -1, int ( * )( ) );

	this->overlay_drawlist = new draw_list_t( );

	menu_event = engine::c_events::get_current_event( )->type( );

	const auto key_code = get_event_keycode_fn( get_current_event_fn( ) );

	insert_state = menu_event == engine::event_type::key_down && key_code == engine::key_code::Insert;
	mouse_state = get_mouse_button_fn( 0 );

	mouse_pos.x = get_mouse_pos_fn( ).x;
	mouse_pos.y = get_screen_height_fn( ) - get_mouse_pos_fn( ).y;

	if ( insert_state && !old_insert_state && !always_open )
		is_menu_open = !is_menu_open;

	if ( is_menu_open )
	{
		menu_alpha = min( menu_alpha + 7, 255 );
	}

	if ( !is_menu_open ) {
		menu_alpha = max( menu_alpha - 7, 0 );
		if ( menu_alpha == 0 )
			return false;
	}

	if ( is_mouse_in_box( { window_pos.x, window_pos.y }, { window_pos.x + window_size.x, window_pos.y + 20 } ) && mouse_state && menu::vars::main_menu_open ) {
		this->window_pos.x += mouse_pos.x - old_mouse_pos.x;
		this->window_pos.y += mouse_pos.y - old_mouse_pos.y;
	}

	if ( menu_event == engine::event_type::repaint )
	{
		engine::gui::filled_rect( { window_pos.x, window_pos.y, window_size.x + 1, window_size.y + 1 }, { 24, 24, 24, menu_alpha } );

		engine::gui::filled_rect( { window_pos.x, window_pos.y, window_size.x + 1, 15 }, { 28, 28, 28, menu_alpha } );

		engine::gl::gradient_filled_rect( window_pos.x, window_pos.y + 16, window_size.x + 1, 10, { 17, 17, 17, 166 }, { 0, 0, 0, 0 } );

		engine::gui::rect( window_pos.x, window_pos.y, window_size.x + 1, window_size.y + 1, { 0, 0, 0, menu_alpha } );

		engine::gui::string( window_pos.x + 6.f, window_pos.y, 300, 20, wstring.c_str( ), { 0, 0, 0, menu_alpha }, false, false );
		engine::gui::string( window_pos.x + 5.f, window_pos.y - 1, 300, 20, wstring.c_str( ), { 255, 255, 255, menu_alpha }, false, false );
	}

	return true;
}

void c_menu_framework::watermark( )
{
	if ( menu_event == engine::event_type::repaint )
	{
		auto date = std::string( __DATE__ );
		std::transform( date.begin( ), date.end( ), date.begin( ), ::tolower );

		auto date_str = std::wstring( date.begin( ), date.end( ) );

		if ( !wakeup )
		{
			auto text = std::wstring( E( L"hooks initialized. build date: " ) + date_str );

			features::notifcations.push( text.c_str( ), unity::c_time::get_time( ) );
			wakeup = true;
		}

		const auto watermark_string = std::wstring( E( L"chloehack" ) );
		engine::gui::string_with_background( 3, 0, 300, 20, watermark_string.c_str( ), { 255, 255, 255, 180 }, true );
	}
}

void c_menu_framework::end_window( ) {

	this->overlay_drawlist->render( );

	old_insert_state = insert_state;
	old_mouse_pos = mouse_pos;
	old_mouse_state = mouse_state;
}

auto resolve_icon( int index ) -> managed_system::c_string {

	switch ( index )
	{
	case 0:
		return E( L"rage.png" );
	case 1:
		return E( L"visual.png" );
	case 2:
		return E( L"misc.png" );
	}

	return E( L"" );
}

void c_menu_framework::text( const std::wstring& text, const geometry::vec2_t& pos ) {

	if ( menu_event == engine::event_type::repaint )
	{
		engine::gui::string( window_pos.x + pos.x + 1, window_pos.y + pos.y + 1, 300, 20, text.c_str( ), { 0, 0, 0, 180 }, false, false );
		engine::gui::string( window_pos.x + pos.x, window_pos.y + pos.y, 300, 20, text.c_str( ), { 255, 255, 255, 180 }, false, false );
	}
}


short c_menu_framework::tabs( const std::vector<std::string> names ) {
	static short selected_tab = 0;

	MONO_METHOD( draw_texture_fn, E( "UnityEngine::GUI.DrawTexture()" ), 2, void ( * )( engine::rect_t, std::uintptr_t ) );
	MONO_METHOD( set_color_fn, E( "UnityEngine::GUI.set_color()" ), -1, void( * )( engine::color_t ) );


	for ( short i = 0; i < names.size( ); i++ )
	{
		const float tab_size = 80;//window_size.x / names.size( );

		//if ( is_mouse_in_box( { window_pos.x + ( i * tab_size ), window_pos.y + 20 }, { window_pos.x + ( i * tab_size ) + tab_size, window_pos.y + 40 } ) && mouse_state )
		if ( is_mouse_in_box( { window_pos.x + 25.f, window_pos.y + ( i * 100 ) + 35.f }, { window_pos.x + tab_size + 25.f, window_pos.y + ( i * 100 ) + 35.f + tab_size } ) && mouse_state )
			selected_tab = i;

		//const auto wstring = std::wstring( names.at( i ).begin( ), names.at( i ).end( ) );


		if ( menu_event == engine::event_type::repaint )
		{
			auto texture = engine::gui::bundle->load_asset<std::uintptr_t>( resolve_icon( i ), mono_lib::type_object( E( "UnityEngine" ), E( "Texture" ) ) );

			set_color_fn( i == selected_tab ? engine::color_t{ 167, 167, 167, menu_alpha } : engine::color_t{ 61, 61, 61, menu_alpha } );
			draw_texture_fn( { window_pos.x + 27.f, window_pos.y + ( i * 100 ) + 50.f + 22.f - 12.f, 48.f, 48.f }, texture );

			engine::gui::rect( window_pos.x + tab_size + 25.f, window_pos.y + ( i * 100 ) + 35.f, 0, 100.f, i == selected_tab ? engine::color_t{ 255, 255, 255, menu_alpha } : engine::color_t{ 52, 52, 52, menu_alpha } );
		}
	}

	return selected_tab;
}

short c_menu_framework::sub_tabs( const std::vector<std::string> names ) {
	static short selected_tab = 0;

	for ( short i = 0; i < names.size( ); i++ ) {

		const float tab_size = 170;

		if ( is_mouse_in_box( { window_pos.x + ( i * tab_size ) + 115, window_pos.y + 30 }, { window_pos.x + ( i * tab_size ) + tab_size + 115, window_pos.y + 45 } ) && mouse_state )
			selected_tab = i;

		const auto wstring = std::wstring( names.at( i ).begin( ), names.at( i ).end( ) );

		if ( menu_event == engine::event_type::repaint )
		{
			engine::gui::rect( window_pos.x + ( i * tab_size ) + 115, window_pos.y + 45, tab_size, 1, i == selected_tab ? engine::color_t{ 255, 255, 255, menu_alpha } : engine::color_t{ 52, 52, 52, menu_alpha } );

			engine::gui::string( window_pos.x + ( i * tab_size ) + tab_size / 2 - 150 + 115 + 1, window_pos.y + 25 + 1, 300, 20, wstring.c_str( ), engine::color_t( 0, 0, 0, menu_alpha ), true, false );
			engine::gui::string( window_pos.x + ( i * tab_size ) + tab_size / 2 - 150 + 115, window_pos.y + 25, 300, 20, wstring.c_str( ), engine::color_t( 255, 255, 255, menu_alpha ), true, false );
		}
	}

	return selected_tab;
}

void c_menu_framework::listbox( const std::string& title, const std::vector<std::string> items, int& selected_index )
{
	const auto wstring = std::wstring( title.begin( ), title.end( ) );

	for ( size_t idx{ 0 }; idx < items.size( ); idx++ ) {
		if ( is_mouse_in_box( { next_item_pos.x, next_item_pos.y + ( idx * 18 ) }, { next_item_pos.x + 100, next_item_pos.y + ( idx * 18 ) + 15 } ) && mouse_state && !old_mouse_state && !interacting )
			selected_index = idx;
	}

	if ( menu_event == engine::event_type::repaint )
	{
		for ( size_t idx{ 0 }; idx < items.size( ); idx++ ) {
			const auto current_item = std::wstring( items.at( idx ).begin( ), items.at( idx ).end( ) );
			auto cleaned = current_item.substr( current_item.find_last_of( L"\\" ) + 1 );

			engine::gui::string( next_item_pos.x + 7 + 1, next_item_pos.y + ( idx * 18 ) + 1, 300, 20, cleaned.c_str( ), engine::color_t{ 0, 0, 0, menu_alpha }, false, false );
			engine::gui::string( next_item_pos.x + 7, next_item_pos.y + ( idx * 18 ), 300, 20, cleaned.c_str( ), idx == selected_index ? engine::color_t{ 255, 255, 255, menu_alpha } : engine::color_t{ 54, 54, 54, menu_alpha }, false, false );
		}
	}

	engine::gui::rect( next_item_pos.x, next_item_pos.y, 130, ( items.size( ) * 18 ) + 10.f, engine::color_t{ 52, 52, 52, menu_alpha } );

	next_item_pos.y += ( items.size( ) * 18 ) + 10.f;
}

void c_menu_framework::combobox( const std::string& title, const geometry::vec2_t& pos, const geometry::vec2_t& size )
{
	const auto wstring = std::wstring( title.begin( ), title.end( ) );

	engine::gui::rect( window_pos.x + pos.x, window_pos.y + pos.y, size.x, size.y, engine::color_t{ 52, 52, 52, menu_alpha } );

	auto t_size = engine::gui::calculate_text_size( wstring.c_str( ) );

	engine::gui::filled_rect( { window_pos.x + pos.x + 5.f + 1 - 3, window_pos.y + pos.y - 10.f + 3, t_size.x + 6, 16 }, { 24, 24, 24, menu_alpha } );

	engine::gui::string( window_pos.x + pos.x + 6.f + 1, window_pos.y + pos.y - 10.f + 1, 300, 20, wstring.c_str( ), { 0, 0, 0, menu_alpha }, false, false );
	engine::gui::string( window_pos.x + pos.x + 6.f, window_pos.y + pos.y - 10.f, 300, 20, wstring.c_str( ), { 255, 255, 255, menu_alpha }, false, false );

	next_item_pos = { window_pos.x + pos.x + 15, window_pos.y + pos.y + 17 };
}

void c_menu_framework::keybind( const std::string& title, int& value ) {
	const auto wstring = std::wstring( title.begin( ), title.end( ) );

	static bool binding_key = false;

	MONO_METHOD( get_event_keycode_fn, E( "UnityEngine::Event.get_keyCode()" ), -1, engine::key_code( * )( std::uintptr_t ) );
	MONO_METHOD( get_event_button_fn, E( "UnityEngine::Event.get_button()" ), -1, int( * )( std::uintptr_t ) );
	MONO_METHOD( get_current_event_fn, E( "UnityEngine::Event.get_current()" ), -1, std::uintptr_t( * )( ) );
	MONO_METHOD( get_is_mouse_event_fn, E( "UnityEngine::Event.get_isMouse()" ), -1, std::uintptr_t( * )( std::uintptr_t ) );

	if ( is_mouse_in_box( { next_item_pos.x - 6, next_item_pos.y + 16 }, { next_item_pos.x + 141 + 6, next_item_pos.y + 16 + 21 } ) && mouse_state && !old_mouse_state )
		binding_key = true;

	if ( menu_event == engine::event_type::key_down && binding_key )
	{
		value = static_cast< int >( get_event_keycode_fn( get_current_event_fn( ) ) );
		if ( value == static_cast< int >( engine::key_code::Escape ) )
			value = 0;

		binding_key = false;
	}

	if ( menu_event == engine::event_type::repaint )
	{
		engine::gui::filled_rect( { next_item_pos.x - 6, next_item_pos.y + 16, 140, 20 }, { 61, 61, 61, menu_alpha } );

		engine::gui::rect( next_item_pos.x - 6, next_item_pos.y + 16, 140, 20, { 52, 52, 52, menu_alpha } );

		engine::gui::string( next_item_pos.x + 2 + 1 - 6, next_item_pos.y - 2 + 1, 300, 20, wstring.c_str( ), { 0, 0, 0, menu_alpha }, false, false );
		engine::gui::string( next_item_pos.x + 2 - 6, next_item_pos.y - 2, 300, 20, wstring.c_str( ), { 255, 255, 255, menu_alpha }, false, false );

		auto key = value == 0 ? std::wstring( E( L"no key" ) ) : std::wstring( E( L"key: " ) ) + std::to_wstring( value ).c_str( );

		engine::gui::string( next_item_pos.x + 1 + 70 - 150 + 1 - 6, next_item_pos.y + 17 + 1, 300, 20, key.c_str( ), { 0, 0, 0, menu_alpha }, true, false );
		engine::gui::string( next_item_pos.x + 1 + 70 - 150 - 6, next_item_pos.y + 17, 300, 20, key.c_str( ), { 255, 255, 255, menu_alpha }, true, false );
	}

	next_item_pos.y += 40;
}

void c_menu_framework::checkbox( const std::string& title, bool& value ) {
	const auto wstring = std::wstring( title.begin( ), title.end( ) );

	if ( is_mouse_in_box( { next_item_pos.x - 6, next_item_pos.y }, { next_item_pos.x - 6 + 15, next_item_pos.y + 15 } ) && mouse_state && !old_mouse_state && !interacting )
		value = !value;

	if ( menu_event == engine::event_type::repaint )
	{
		engine::gui::filled_rect( { next_item_pos.x - 6, next_item_pos.y + 1, 16, 16 }, { 24, 24, 24, menu_alpha } );
		if ( value )
			engine::gui::filled_rect( { next_item_pos.x - 6, next_item_pos.y + 1, 16, 16 }, { 171, 171, 171, menu_alpha } );

		engine::gui::rect( next_item_pos.x - 7, next_item_pos.y, 17, 17, { 61, 61, 61, menu_alpha } );

		engine::gui::string( next_item_pos.x + 15.f + 1, next_item_pos.y + 1 + 1, 300, 20, wstring.c_str( ), { 0, 0, 0, menu_alpha }, false, false );
		engine::gui::string( next_item_pos.x + 15.f, next_item_pos.y + 1, 300, 20, wstring.c_str( ), { 255, 255, 255, menu_alpha }, false, false );
	}

	next_item_pos.y += 22;
}

void c_menu_framework::slider( const std::string& title, int& value, int min, int max, const std::string& suffix )
{
	auto wstring = std::wstring( title.begin( ), title.end( ) );
	auto wstring_suffix = std::wstring( suffix.begin( ), suffix.end( ) );


	next_item_pos.x -= 6;

	if ( menu_event == engine::event_type::repaint )
	{
		wstring.append( E( L": " ) + std::to_wstring( value ) + wstring_suffix );

		if ( is_mouse_in_box( { next_item_pos.x + 1, next_item_pos.y + 16 }, { next_item_pos.x + 141, next_item_pos.y + 22 } ) && mouse_state && !interacting ) {
			value = int( ( old_mouse_pos.x - ( next_item_pos.x + 1 ) ) * ( max - min ) / 140 );
		}

		engine::gui::string( next_item_pos.x + 1 + 1, next_item_pos.y - 2 + 1, 300, 20, wstring.c_str( ), { 0, 0, 0, menu_alpha }, false, false );
		engine::gui::string( next_item_pos.x + 1, next_item_pos.y - 2, 300, 20, wstring.c_str( ), { 255, 255, 255, menu_alpha }, false, false );

		engine::gui::filled_rect( { next_item_pos.x, next_item_pos.y + 16, 140, 5 }, { 61, 61, 61, menu_alpha } );
		engine::gui::filled_rect( { next_item_pos.x + 1, next_item_pos.y + 16, static_cast< float >( ( value * 140 ) / max ) - 1, 5 }, { 171, 171, 171, menu_alpha } );
	}

	next_item_pos.x += 6;
	next_item_pos.y += 28;
}

void c_menu_framework::button( const std::string& title, void* callback ) {

	const auto wstring = std::wstring( title.begin( ), title.end( ) );

	if ( menu_event == engine::event_type::repaint )
		engine::gui::filled_rect( { next_item_pos.x - 6, next_item_pos.y + 16, 140, 20 }, { 61, 61, 61, menu_alpha } );


	if ( is_mouse_in_box( { next_item_pos.x - 25, next_item_pos.y + 16 }, { next_item_pos.x - 25 + 196, next_item_pos.y + 16 + 20 } ) && mouse_state && !old_mouse_state )
	{
		if ( menu_event == engine::event_type::repaint )
			engine::gui::filled_rect( { next_item_pos.x - 6, next_item_pos.y + 16, 140, 20 }, { 23, 23, 23, menu_alpha } );
		
		if ( callback != 0x0 )
			reinterpret_cast< void( * )( void ) >( callback )( );
	}

	if ( menu_event == engine::event_type::repaint )
	{
		engine::gui::rect( next_item_pos.x - 6, next_item_pos.y + 16, 140, 20, { 52, 52, 52, menu_alpha } );

		engine::gui::string( next_item_pos.x - 6 + 140 / 2 - 150 + 1, next_item_pos.y + 17 + 1, 300, 25, wstring.c_str( ), { 0, 0, 0, menu_alpha }, true, false );
		engine::gui::string( next_item_pos.x - 6 + 140 / 2 - 150, next_item_pos.y + 17, 300, 25, wstring.c_str( ), engine::color_t{ 255, 255, 255, menu_alpha }, true, false );

	}

	next_item_pos.y += 25;
}

void c_menu_framework::colorpicker( const std::string& title, engine::color_t& value, bool& is_open, bool inlined ) {

	if ( !inlined ) {
		this->next_item_pos.y += 15.f;
	}
}

void c_menu_framework::dropdown( const std::string& title, const std::vector<std::string> items, int& value, bool& is_open ) {
	const auto wstring = std::wstring( title.begin( ), title.end( ) );

	if ( is_mouse_in_box( { next_item_pos.x - 6, next_item_pos.y + 16 }, { next_item_pos.x + 140 - 6, next_item_pos.y + 15 + 16 } ) && mouse_state && !old_mouse_state && !interacting ) {
		is_open = true;
		interacting = true;
	}

	if ( menu_event == engine::event_type::repaint )
	{
		engine::gui::string( next_item_pos.x - 6 + 2 + 1, next_item_pos.y + 1, 300, 50, wstring.c_str( ), { 0, 0, 0, menu_alpha }, false, false );
		engine::gui::string( next_item_pos.x - 6 + 2, next_item_pos.y, 300, 50, wstring.c_str( ), { 255, 255, 255, menu_alpha }, false, false );

		if ( is_open && !is_mouse_in_box( { next_item_pos.x - 6, next_item_pos.y + 16 }, { next_item_pos.x + 140 - 6, next_item_pos.y + 1 + 13 + items.size( ) * 15 + 16 } ) && mouse_state ) {
			is_open = false;
			interacting = false;
		}

		const auto str = std::wstring( items.at( value ).begin( ), items.at( value ).end( ) );

		if ( is_open )
		{
			this->overlay_drawlist->rect( { next_item_pos.x - 6, next_item_pos.y + 16 + 1, 140, 15 }, { 61, 61, 61, 255 } );
			this->overlay_drawlist->rect( { next_item_pos.x - 6, next_item_pos.y + 1 + 14 + 16, 140, items.size( ) * 15.f }, { 27, 27, 27, 255 } );

			for ( auto i{ 0 }; i < items.size( ); i++ ) {

				const auto cur = std::wstring( items.at( i ).begin( ), items.at( i ).end( ) );

				if ( value == i )
				{
					this->overlay_drawlist->rect( { next_item_pos.x - 6, next_item_pos.y + 1 + 14 + ( i * 15 ) + 16, 140, 15 }, { 171, 171, 171, 255 } );
				}

				if ( is_mouse_in_box( { next_item_pos.x - 6, next_item_pos.y + 1 + 13 + ( i * 15 ) + 16 }, { next_item_pos.x + 140 - 6, next_item_pos.y + 1 + 23 + ( i * 15 ) + 10 + 16 } ) )
				{
					if ( mouse_state )
					{
						value = i;
						is_open = false;
						interacting = false;
					}
				}

				this->overlay_drawlist->box( { next_item_pos.x - 6, next_item_pos.y + 1 + 14 + ( i * 15 ) + 16 }, { 140, 15 }, { 52, 52, 52, 255 } );

				this->overlay_drawlist->text( { next_item_pos.x + 1, next_item_pos.y + 1 + 15 + ( i * 15 ) - 2 + 16 + 1, 300, 50 }, cur.c_str( ), { 0, 0, 0, 255 }, false, false );
				this->overlay_drawlist->text( { next_item_pos.x, next_item_pos.y + 1 + 15 + ( i * 15 ) - 2 + 16, 300, 50 }, cur.c_str( ), { 255, 255, 255, 255 }, false, false );
			}

			this->overlay_drawlist->box( { next_item_pos.x - 6, next_item_pos.y + 16 }, { 140, 15 }, { 52, 52, 52, 255 } );

			this->overlay_drawlist->text( { next_item_pos.x + 1, next_item_pos.y + 1 - 1 + 16 + 1, 300, 50 }, str.c_str( ), { 0, 0, 0, 255 }, false, false );
			this->overlay_drawlist->text( { next_item_pos.x, next_item_pos.y + 1 - 1 + 16, 300, 50 }, str.c_str( ), { 255, 255, 255, 255 }, false, false );
		}
		else {
			engine::gui::filled_rect( { next_item_pos.x - 6, next_item_pos.y + 1 + 16, 140, 15 }, { 61, 61, 61, menu_alpha } );

			engine::gui::rect( next_item_pos.x - 6, next_item_pos.y + 16, 140, 15, { 52, 52, 52, menu_alpha } );

			engine::gui::string( next_item_pos.x + 1, next_item_pos.y + 1 - 1 + 16 + 1, 300, 50, str.c_str( ), { 0, 0, 0, menu_alpha }, false, false );
			engine::gui::string( next_item_pos.x, next_item_pos.y + 1 - 1 + 16, 300, 50, str.c_str( ), { 255, 255, 255, menu_alpha }, false, false );
		}
	}

	next_item_pos.y += 40;
}


void c_menu_framework::multi_dropdown( const std::string& title, const std::vector<std::string> items, std::vector<bool>& value, bool& is_open ) {
	const auto wstring = std::wstring( title.begin( ), title.end( ) );

	if ( is_mouse_in_box( { next_item_pos.x - 6, next_item_pos.y + 16 }, { next_item_pos.x + 140 - 6, next_item_pos.y + 15 + 16 } ) && mouse_state && !old_mouse_state && !interacting ) {
		is_open = true;
		interacting = true;
	}

	if ( is_open ) 
	{
		for ( size_t i{ 0 }; i < items.size( ); i++ ) 
		{


			if ( is_mouse_in_box( { next_item_pos.x - 6, next_item_pos.y + 1 + 13 + ( i * 15 ) + 16 }, { next_item_pos.x + 140 - 6, next_item_pos.y + 1 + 23 + ( i * 15 ) + 10 + 16 - 2 } ) ) {
				if ( mouse_state && !old_mouse_state )
				{
					value[ i ] = !value[ i ];
				}
			}
		}
	}

	if ( menu_event == engine::event_type::repaint )
	{
		engine::gui::string( next_item_pos.x - 6 + 2 + 1, next_item_pos.y + 1, 300, 50, wstring.c_str( ), { 0, 0, 0, menu_alpha }, false, false );
		engine::gui::string( next_item_pos.x - 6 + 2, next_item_pos.y, 300, 50, wstring.c_str( ), { 255, 255, 255, menu_alpha }, false, false );

		if ( is_open && !is_mouse_in_box( { next_item_pos.x - 6, next_item_pos.y + 16 }, { next_item_pos.x + 140 - 6, next_item_pos.y + 1 + 13 + items.size( ) * 15 + 16 } ) && mouse_state ) {
			is_open = false;
			interacting = false;
		}

		std::wstring str;

		for ( size_t i{ 0 }; i < items.size( ); i++ ) {
			if ( value[ i ] ) {
				auto wstr = std::wstring( items.at( i ).begin( ), items.at( i ).end( ) );
				if ( str.length( ) < 15 && i != items.size( ) - 1 )
					str.append( wstr + E( L", " ) );
				else if ( str.length( ) < 15 )
					str.append( wstr );
				else {
					str.append( E( L"..." ) );
					break;
				}
			}
		}

		if ( is_open )
		{
			this->overlay_drawlist->rect( { next_item_pos.x - 6, next_item_pos.y + 16 + 1, 140, 15 }, { 61, 61, 61, 255 } );
			this->overlay_drawlist->rect( { next_item_pos.x - 6, next_item_pos.y + 1 + 14 + 16, 140, items.size( ) * 15.f }, { 27, 27, 27, 255 } );

			for ( size_t i{ 0 }; i < items.size( ); i++ ) {

				const auto cur = std::wstring( items.at( i ).begin( ), items.at( i ).end( ) );

				if ( value[ i ] )
				{
					this->overlay_drawlist->rect( { next_item_pos.x - 6, next_item_pos.y + 1 + 14 + ( i * 15 ) + 16, 140, 15 }, { 171, 171, 171, 255 } );
				}

				this->overlay_drawlist->box( { next_item_pos.x - 6, next_item_pos.y + 1 + 14 + ( i * 15 ) + 16 }, { 140, 15 }, { 52, 52, 52, 255 } );

				this->overlay_drawlist->text( { next_item_pos.x + 1, next_item_pos.y + 1 + 15 + ( i * 15 ) - 2 + 1 + 16, 300, 50 }, cur.c_str( ), { 0, 0, 0, 255 }, false, false );
				this->overlay_drawlist->text( { next_item_pos.x, next_item_pos.y + 1 + 15 + ( i * 15 ) - 2 + 16, 300, 50 }, cur.c_str( ), { 255, 255, 255, 255 }, false, false );
			}

			this->overlay_drawlist->box( { next_item_pos.x - 6, next_item_pos.y + 16 }, { 140, 15 }, { 52, 52, 52, 255 } );

			this->overlay_drawlist->text( { next_item_pos.x + 1, next_item_pos.y + 1 - 1 + 16 + 1, 300, 50 }, str.empty( ) ? E( L"none" ) : str.c_str( ), { 0, 0, 0, 255 }, false, false );
			this->overlay_drawlist->text( { next_item_pos.x, next_item_pos.y + 1 - 1 + 16, 300, 50 }, str.empty( ) ? E( L"none" ) : str.c_str( ), { 255, 255, 255, 255 }, false, false );
		}

		else {
			engine::gui::filled_rect( { next_item_pos.x - 6, next_item_pos.y + 1 + 16, 140, 15 }, { 61, 61, 61, menu_alpha } );
			engine::gui::rect( next_item_pos.x - 6, next_item_pos.y + 16, 140, 15, { 52, 52, 52, menu_alpha } );

			engine::gui::string( next_item_pos.x + 1, next_item_pos.y + 1 - 1 + 16 + 1, 300, 50, str.empty( ) ? E( L"none" ) : str.c_str( ), { 0, 0, 0, menu_alpha }, false, false );
			engine::gui::string( next_item_pos.x, next_item_pos.y + 1 - 1 + 16, 300, 50, str.empty( ) ? E( L"none" ) : str.c_str( ), { 255, 255, 255, menu_alpha }, false, false );
		}
	}

	next_item_pos.y += 40;
}

bool c_menu_framework::is_mouse_in_box( const geometry::vec2_t& pos1, const geometry::vec2_t& pos2 ) {
	if ( old_mouse_pos.x < pos1.x || old_mouse_pos.y < pos1.y )
		return false;

	if ( old_mouse_pos.x > pos2.x || old_mouse_pos.y > pos2.y )
		return false;

	return true;
}