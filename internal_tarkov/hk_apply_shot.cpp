#include "hooks.hpp"
#include "engine_render.hpp"
#include "menu.hpp"
#include "notifications.hpp"

std::uintptr_t game_object, audio_source;

auto resolve_bodypart_type( int type ) -> std::wstring {

	switch ( type )
	{
	case 0:
		return E( L" in the Head" );
	case 1:
		return E( L" in the Chest" );
	case 2:
		return E( L" in the Stomach" );
	case 3:
		return E( L" in the Left Arm" );
	case 4:
		return E( L" in the Right Arm" );
	case 5:
		return E( L" in the Left Leg" );
	case 6:
		return E( L" in the Right Leg" );
	case 7:
		return E( L" in Common" );
	}
}




auto impl::hooks::hk_apply_shot( c_player* instance, hit_info_t* damageInfo, int bodyPartType, void* shotId ) -> std::uintptr_t
{
	auto victim = instance;
	auto victim_name = victim->profile( )->info( )->name( );

	auto player = damageInfo->player;
	auto damage_type = damageInfo->type;
	auto damage = damageInfo->damage;

	if ( damage_type == 512 && player == engine::local_player )
	{
		auto part = resolve_bodypart_type( bodyPartType );

		const auto notification_string = std::wstring( E( L"Hit " ) ) + std::wstring( victim_name ) + part + E( L" for " ) + std::to_wstring( static_cast< int >( damage ) ) + E( L" damage." );

		MONO_METHOD( play_one_shot_fn, E( "UnityEngine::AudioSource.PlayOneShot()" ), 2, void( * )( std::uintptr_t, std::uintptr_t, float ) );
		const auto audio_clip = engine::gui::bundle->load_asset<std::uintptr_t>( E( L"hit.wav" ), mono_lib::type_object( E( "UnityEngine" ), E( "AudioClip" ) ) );

		features::notifcations.push( notification_string.c_str( ), unity::c_time::get_time( ) );

		if ( !game_object || !audio_source ) {
			game_object = mono_lib::object_new( mono_lib::get_root_domain( ), mono_lib::find_class( E( "GameObject" ), E( "UnityEngine" ) ) );
			engine::game_object::create_game_object( game_object, E( L"" ) );
			audio_source = engine::game_object::add_component( game_object, mono_lib::type_object( E( "UnityEngine" ), E( "AudioSource" ) ) );
			engine::object::dont_destroy_on_load( game_object );
		}

		if ( audio_source ) {
			play_one_shot_fn( audio_source, audio_clip, 0.1f );
		}

		m_tracers.push_back( tracer_t{ damageInfo->hit_origin, damageInfo->hit_point, unity::c_time::get_time( ) } );
	}

	impl::hooks::apply_shot.toggle_hook( true );
	auto ret = impl::hooks::apply_shot.get_original<decltype( &impl::hooks::hk_apply_shot )>( )( instance, damageInfo, bodyPartType, shotId );
	impl::hooks::apply_shot.toggle_hook( false );

	return ret;
}