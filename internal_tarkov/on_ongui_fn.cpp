#include "hooks.hpp"
#include "engine_render.hpp"
#include "sdk.hpp"
#include "visuals.hpp"
#include "aimbot.hpp"
#include "menu.hpp"
#include "misc.hpp"
#include "weapon.hpp"
#include "xorstr.hpp"
#include "notifications.hpp"

#include "unity.hpp"

auto impl::hooks::hk_ongui_fn( void* instance ) -> void
{
	engine::gui::setup( );
	engine::gl::setup( );

	{
		engine::game_world = c_world::find_instance( );
		if ( !engine::mem::valid_pointer( ( void* )engine::game_world ) ) 
			goto end;


		auto players = engine::game_world->get_players( );
		if ( !engine::mem::valid_pointer( ( void* )players ) ) {
			engine::game_world = nullptr;
			goto end;
		}

		auto player_count = players->get_count( );
		if ( player_count <= 0 || player_count > 10000 ) {
			engine::game_world = nullptr;
			goto end;
		}

		engine::local_player = reinterpret_cast< c_player* >( players->get( 0 ) );
		if ( !engine::mem::valid_pointer( ( void* )engine::local_player ) ) {
			engine::game_world = nullptr;
			goto end;
		}

		engine::game_camera = engine::objects::get_fps_camera( );
		if ( !engine::mem::valid_pointer( ( void* )engine::game_camera ) )
		{
			engine::game_world = nullptr;
			goto end;
		}


		if ( engine::c_events::get_current_event( )->type( ) == engine::event_type::repaint )
		{
			engine::gui::string( 20, 20, 300, 20, E( L"xerus" ), { 255, 255, 255, 255 }, false, false );

			for ( int idx = 0; idx < player_count; idx++ )
			{
				const auto player = reinterpret_cast< c_player* >( players->get( idx ) );
				if ( !engine::mem::valid_pointer( player ) )
					continue;

				features::visuals.render_player( player );
			}

			const auto exit_controller = *reinterpret_cast< std::uintptr_t* >( engine::game_world + 0x18 );
			if ( !engine::mem::valid_pointer( ( void* )exit_controller ) )
				goto end;

			const auto exit_base = *reinterpret_cast< std::uintptr_t* >( exit_controller + 0x20 );
			if ( !engine::mem::valid_pointer( ( void* )exit_base ) )
				goto end;

			const auto exit_count = *reinterpret_cast< int* >( exit_base + 0x18 );

			if ( exit_count <= 0 || exit_count > 10000 )
				goto end;

			for ( int idx = 0; idx < exit_count; idx++ )
			{
				const auto extract = *reinterpret_cast< std::uintptr_t* >( exit_base + ( 0x20 + ( idx * 8 ) ) );
				if ( !engine::mem::valid_pointer( ( void* )extract ) )
					continue;

				features::visuals.render_extract( extract );
			}

			const auto loot_list = *reinterpret_cast< std::uintptr_t* >( engine::game_world + 0x70 );
			if ( !engine::mem::valid_pointer( ( void* )loot_list ) )
				goto end;

			const auto secondValue = *reinterpret_cast< uint64_t* >( loot_list + 0x18 );
			if ( !engine::mem::valid_pointer( ( void* )secondValue ) )
				goto end;

			const auto loot_base = *reinterpret_cast< std::uintptr_t* >( secondValue + 0x10 );
			if ( !engine::mem::valid_pointer( ( void* )loot_base ) )
				goto end;

			const auto loot_count = *reinterpret_cast< int* >( loot_base + 0x18 );

			if ( loot_count <= 0 || loot_count > 100000 )
				goto end;

			for ( int idx = 0; idx < loot_count; idx++ )
			{
				const auto item = *reinterpret_cast< std::uintptr_t* >( loot_base + ( 0x20 + ( idx * 8 ) ) );
				if ( !engine::mem::valid_pointer( ( void* )item ) )
					continue;

				features::visuals.render_loot( item );
			}

			if ( menu::vars::fov_circle )
			{
				engine::gl::circle( unity::c_screen::get_width( ) / 2.f, unity::c_screen::get_height( ) / 2.f, menu::vars::aimbot_fov, { 255, 255, 255, 255 } );
			}

			const auto camera = engine::screen::c_camera::get_main( );		
			if ( camera ) 
			{
				const static auto old_ratio = camera->get_aspect_ratio( ); // hehe L + ratio 
				if ( menu::vars::aspect_ratio_changer )
					camera->set_aspect_ratio( 1.0f + ( menu::vars::aspect_ratio / 100.f ) );
				else
					camera->set_aspect_ratio( old_ratio );
			}
			
			features::notifcations.run( );
		}

		features::aimbot.find_aimbot_target( );

		if ( features::aimbot.aimbot_target && menu::vars::aimbot_enabled )
			features::aimbot.run_aimbot( );

		features::weapon.weapon_removals( );

		if ( menu::vars::flyhack )
			features::misc.flyhack( );

		features::misc.speedhack( engine::game_world );

		if ( menu::vars::inf_stamina )
			features::misc.no_stamina( );

		if ( menu::vars::max_search_stat )
			features::misc.max_search_stat( );

		if ( menu::vars::instant_ads )
			features::weapon.instant_aim( );

		if ( menu::vars::camera_fx.at( 0 ) )
			features::misc.no_visor( );

		features::misc.night_vision( );

		features::misc.thermal_vision( );

		features::misc.loot_thru_walls( );
	}
	
end:
	menu::render( );

	return;
}