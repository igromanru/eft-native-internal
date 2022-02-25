#include "misc.hpp"
#include "menu.hpp"
#include "unity.hpp"

auto features::c_misc::flyhack( ) -> void
{
	if ( !engine::mem::valid_pointer( engine::game_world ) )
		return;

	if ( !engine::mem::valid_pointer( engine::local_player ) )
		return;

	const auto movement_context = engine::local_player->movement_context( );

	if ( !engine::mem::valid_pointer( movement_context ) )
		return;

	if ( menu::vars::flyhack )
		*reinterpret_cast< float* >( movement_context + 0x1F8 ) = -0.00f;
}

auto features::c_misc::speedhack( ) -> void
{
	if ( !engine::mem::valid_pointer( engine::game_world ) )
		return;

	if ( !engine::mem::valid_pointer( engine::local_player ) )
		return;

	if ( menu::vars::speedhack && engine::get_key( ( engine::key_code )menu::vars::speedhack_key ) )
		unity::c_time::set_timescale( 1.0f + ( menu::vars::speedhack_amnt / 100.f ) );
	else
		unity::c_time::set_timescale( 1.0f );
}

auto features::c_misc::night_vision( ) -> void
{
	if ( !engine::mem::valid_pointer( engine::game_world ) )
		return;

	if ( !engine::mem::valid_pointer( engine::local_player ) )
		return;


	MONO_OBFUSCATED_METHOD( get_instance, E( "Assembly-CSharp" ), E( "\\uE77B" ), E( "get_Instance" ), std::uintptr_t( * )( ) );
	
	MONO_OBFUSCATED_METHOD( get_night_vision, E( "Assembly-CSharp" ), E( "\\uE77B" ), E( "get_NightVision" ), std::uintptr_t( * )( std::uintptr_t ) );
	
	auto instance = get_instance( );
	if ( !engine::mem::valid_pointer( instance ) )
		return;

	auto lol = get_night_vision( instance );
	if ( !engine::mem::valid_pointer( lol ) )
		return;

	*reinterpret_cast< bool* >( lol + 0xCC ) = menu::vars::camera_fx[ 1 ];
}

auto features::c_misc::thermal_vision( ) -> void
{
	if ( !engine::mem::valid_pointer( engine::game_world ) )
		return;

	if ( !engine::mem::valid_pointer( engine::local_player ) )
		return;

	MONO_OBFUSCATED_METHOD( get_instance, E( "Assembly-CSharp" ), E( "\\uE77B" ), E( "get_Instance" ), std::uintptr_t( * )( ) );
	MONO_OBFUSCATED_METHOD( get_thermal_vision, E( "Assembly-CSharp" ), E( "\\uE77B" ), E( "get_ThermalVision" ), std::uintptr_t( * )( std::uintptr_t ) );

	auto instance = get_instance( );
	if ( !engine::mem::valid_pointer( instance ) )
		return;

	auto lol = get_thermal_vision( instance );
	if ( !engine::mem::valid_pointer( lol ) )
		return;

	*reinterpret_cast< bool* >( lol + 0xD0 ) = menu::vars::camera_fx[ 2 ];
}

auto features::c_misc::no_visor( ) -> void
{
	if ( !engine::mem::valid_pointer( engine::game_world ) )
		return;

	if ( !engine::mem::valid_pointer( engine::local_player ) )
		return;

	MONO_OBFUSCATED_METHOD( get_instance, E( "Assembly-CSharp" ), E( "\\uE77B" ), E( "get_Instance" ), std::uintptr_t( * )( ) );
	MONO_OBFUSCATED_METHOD( get_visor_effect, E( "Assembly-CSharp" ), E( "\\uE77B" ), E( "get_VisorEffect" ), std::uintptr_t( * )( std::uintptr_t ) );

	auto instance = get_instance( );
	if ( !engine::mem::valid_pointer( instance ) )
		return;

	auto lol = get_visor_effect( instance );
	if ( !engine::mem::valid_pointer( lol ) )
		return;

	*reinterpret_cast< float* >( lol + 0xB8 ) = 0.f;
}

auto features::c_misc::no_stamina( ) -> void
{
	if ( !engine::mem::valid_pointer( engine::game_world ) )
		return;

	if ( !engine::mem::valid_pointer( engine::local_player ) )
		return;

	const auto phyiscal = *reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( engine::local_player ) + 0x4C8 );
	if ( !engine::mem::valid_pointer( phyiscal ) )
		return;

	const auto stamina = *reinterpret_cast< std::uintptr_t* >( phyiscal + 0x38 );

	if ( !engine::mem::valid_pointer( stamina ) )
		return;

	*reinterpret_cast< float* >( stamina + 0x48 ) = 99.f;
}

auto features::c_misc::max_search_stat( ) -> void
{
	if ( !engine::mem::valid_pointer( engine::game_world ) )
		return;

	if ( !engine::mem::valid_pointer( engine::local_player ) )
		return;

	const auto profile = engine::local_player->profile( );
	if ( !engine::mem::valid_pointer( profile ) )
		return;

	auto skills_ptr = *reinterpret_cast< std::uintptr_t* >(reinterpret_cast<std::uintptr_t>(profile) + 0x60 );
	if ( !engine::mem::valid_pointer( skills_ptr ) )
		return;

	auto search_buff_speed = *reinterpret_cast< std::uintptr_t* >( skills_ptr + 0x470 );
	if ( !engine::mem::valid_pointer( search_buff_speed ) )
		return;

	auto search_double = *reinterpret_cast< std::uintptr_t* >( skills_ptr + 0x478 );
	if ( !engine::mem::valid_pointer( search_double ) )
		return;

	*reinterpret_cast< bool* >( search_double + 0x28 ) = true;
	*reinterpret_cast< bool* >( search_buff_speed + 0x28 ) = true;
}