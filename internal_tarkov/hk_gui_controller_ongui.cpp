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
#include "script_manager.hpp"

//std::vector < std::uintptr_t > paimon_frames;

//bool setup_paimon{ false };
//float next_frame{ 0 }, counter{ 0 };

/*
void paimon( geometry::vec2_t position, float alpha )
{
	const static auto paimon_bundle = unity::c_asset_bundle::load_from_file( E( L"C:\\sapphire\\paimon.bundle" ) );

	if ( !setup_paimon && paimon_bundle )
	{
		for ( int idx = 0; idx < 52; idx++ )
		{
			const auto name = std::to_wstring( idx ) + std::wstring( E( L".png" ) );
			const auto frame = paimon_bundle->load_asset<std::uintptr_t>( name.c_str( ), mono_lib::type_object( E( "UnityEngine" ), E( "Texture" ) ) );

			engine::object::dont_destroy_on_load( frame );

			paimon_frames.push_back( frame );
		}

		setup_paimon = true;
	}

	if ( paimon_frames.empty( ) )
		return;

	auto time = std::floor( unity::c_time::get_time( ) * 1000 );

	if ( next_frame - time > 30 )
		next_frame = 0;

	if ( next_frame - time < 1 )
	{
		counter++;
		next_frame = time + 30;
	}

	auto gif_frame = paimon_frames[ ( int( counter / 1.2 ) % paimon_frames.size( ) ) ];


	MONO_METHOD( set_color_fn, E( "UnityEngine::GUI.set_color()" ), -1, void( * )( engine::color_t ) );
	MONO_METHOD( draw_texture_fn, E( "UnityEngine::GUI.DrawTexture()" ), 2, void ( * )( engine::rect_t, std::uintptr_t ) );

	set_color_fn( { 255, 255, 255, alpha } );
	draw_texture_fn( { position.x, position.y, 200, 200 }, gif_frame );
}
*/
auto impl::hooks::hk_gui_controller_ongui( void* instance ) -> void
{
	engine::gui::setup( );
	engine::gl::setup( );

	{
		engine::game_world = c_world::find_instance( );
		if ( !engine::mem::valid_pointer( engine::game_world ) ) {
			engine::game_world = nullptr;
			engine::local_player = nullptr;
			goto end;
		}

		auto players = engine::game_world->get_players( );
		if ( !engine::mem::valid_pointer( players ) ) {
			engine::game_world = nullptr;
			engine::local_player = nullptr;
			goto end;
		}

		auto player_count = players->get_count( );
		if ( player_count <= 0 || player_count > 10000 ) {
			engine::game_world = nullptr;
			engine::local_player = nullptr;
			goto end;
		}

		engine::local_player = reinterpret_cast< c_player* >( players->get( 0 ) );
		if ( !engine::mem::valid_pointer( engine::local_player ) ) {
			engine::game_world = nullptr;
			engine::local_player = nullptr;
			goto end;
		}

		if ( engine::c_events::get_current_event( )->type( ) == engine::event_type::repaint )
		{
			for ( int idx = 0; idx < player_count; idx++ )
			{
				const auto player = reinterpret_cast< c_player* >( players->get( idx ) );
				if ( !engine::mem::valid_pointer( player ) )
					continue;

				features::visuals.render_player( player );
			}


			const auto exit_controller = *reinterpret_cast< std::uintptr_t* >( engine::game_world + 0x18 );
			if ( !engine::mem::valid_pointer( exit_controller ) )
				goto end;

			const auto exit_base = *reinterpret_cast< std::uintptr_t* >( exit_controller + 0x20 );
			if ( !engine::mem::valid_pointer( exit_base ) )
				goto end;

			const auto exit_count = *reinterpret_cast< int* >( exit_base + 0x18 );
			if ( exit_count <= 0 || exit_count > 10000 )
				goto end;

			for ( int idx = 0; idx < exit_count; idx++ )
			{
				const auto extract = *reinterpret_cast< std::uintptr_t* >( exit_base + ( 0x20 + ( idx * 8 ) ) );
				if ( !engine::mem::valid_pointer( extract ) )
					continue;

				features::visuals.render_extract( extract );
			}

			if ( menu::vars::widget )
			{
				features::visuals.local_ammo( );
			}


			if ( menu::vars::fov_circle )
			{
				engine::gl::circle( unity::c_screen::get_width( ) / 2.f, unity::c_screen::get_height( ) / 2.f, menu::vars::aimbot_fov, menu::vars::fov_color );
			}

			const auto camera = unity::c_camera::get_main( );
			if ( camera )
			{
				const static auto old_ratio = camera->get_aspect_ratio( ); // hehe L + ratio
				if ( menu::vars::aspect_ratio_changer )
					camera->set_aspect_ratio( 1.0f + ( menu::vars::aspect_ratio / 100.f ) );
				else
					camera->set_aspect_ratio( old_ratio );
			}

			if ( menu::vars::hitmarker )
			{
				if ( !m_tracers.empty( ) )
				{


					float time = unity::c_time::get_time( );


					for ( size_t i{ }; i < m_tracers.size( ) && !m_tracers.empty( ); ++i ) {
						auto& tr = m_tracers[ i ];

						float delta = time - tr.time;
						if ( delta > 1.0f || std::abs( delta ) > 5.f ) m_tracers.erase( m_tracers.begin( ) + i );
					}

					for ( auto& it : m_tracers )
					{
						float delta = time - it.time;

						auto col = engine::color_t::from_hsb( delta, 1.0f, 1.0f );
						col.a = 1;

						engine::gl::in_world_line( it.start, it.end, col );
					}
				}
			}
		}

		features::aimbot.find_aimbot_target( );

		if ( features::aimbot.aimbot_target && menu::vars::aimbot_enabled )
			features::aimbot.run_aimbot( );

		features::weapon.weapon_removals( );

		if ( menu::vars::flyhack )
			features::misc.flyhack( );

		features::misc.speedhack( );

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
	}

end:
	if ( engine::c_events::get_current_event( )->type( ) == engine::event_type::repaint )
		features::notifcations.run( );

	menu::render( );

	scripting::object.run_paint_callbacks( );

	return;
}