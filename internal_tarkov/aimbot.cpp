#include "aimbot.hpp"
#include "menu.hpp"
#include "unity.hpp"

auto calc_angle( const geometry::vec3_t f, const geometry::vec3_t t ) -> geometry::vec3_t {
	auto delta = f - t;

	constexpr auto r2d = 57.2957795131f;
	constexpr auto d2r = 0.01745329251f;

	const auto pitch = unity::c_math::asin( delta.y / delta.length( ) );
	const auto yaw = -unity::c_math::atan2( delta.x, -delta.z );

	return geometry::vec3_t{ yaw, pitch, 0.f } *r2d;
}

//"mouse 1", "mouse 2", "mouse 4", "mouse 5"
auto get_aimkey_index( int menu_index ) -> int {
	switch ( menu_index )
	{
	case 0:
		return 0;
	case 1:
		return 1;
	case 2:
		return 3;
	case 3:
		return 4;
	}

	return 0;
}

auto features::c_aimbot::get_aimbone_index( int menu_index ) -> int
{
	// head, neck, chest, pelvis
	switch ( menu_index ) 
	{
	case 0:
		return 133;
	case 1:
		return 132;
	case 2:
		return 29;
	case 3:
		return 14;
	}

	return 133;
}

auto features::c_aimbot::find_aimbot_target( ) -> void
{
	MONO_METHOD( get_mouse_button_fn, E( "UnityEngine::Input.GetMouseButton()" ), 1, bool( * )( int ) );

	if ( !engine::mem::valid_pointer( engine::game_world ) )
		return;

	if ( !engine::mem::valid_pointer( engine::local_player ) )
		return;

	aimbot_target = nullptr;
	auto best_fov = float( menu::vars::aimbot_fov );

	auto players = engine::game_world->get_players( );
	if ( !engine::mem::valid_pointer( ( void* )players ) )
		return;

	auto player_count = players->get_count( );
	if ( player_count <= 0 || player_count > 10000 )
		return;

	for ( int idx = 0; idx < player_count; idx++ )
	{
		const auto player = reinterpret_cast< c_player* >( players->get( idx ) );
		if ( !engine::mem::valid_pointer( player ) )
			return;

		auto bone_transform = player->bone_transform<unity::c_transform*>( get_aimbone_index( menu::vars::aim_bone ) );
		if ( !engine::mem::valid_pointer( bone_transform ) )
			return;

		auto aim_position = bone_transform->position( );
		if ( aim_position.empty( ) )
			return;

		const auto fov = engine::screen::calculate_fov( aim_position );

		if ( fov < best_fov )
		{
			aimbot_target = player;
			best_fov = fov;
		}
	}
}

auto features::c_aimbot::run_aimbot( ) -> void
{
	MONO_METHOD( get_mouse_button_fn, E( "UnityEngine::Input.GetMouseButton()" ), 1, bool( * )( int ) );

	if ( !engine::mem::valid_pointer( engine::local_player ) )
		return;

	if ( !engine::mem::valid_pointer( aimbot_target ) )
		return;

	if ( menu::vars::aim_type != 0 )
		return;

	const auto hands_controller = *reinterpret_cast< std::uintptr_t* >( engine::local_player + 0x508 );
	if ( !engine::mem::valid_pointer( hands_controller ) )
		return;

	const auto fireport = *reinterpret_cast< std::uintptr_t* >( hands_controller + 0xe8 );
	if ( !engine::mem::valid_pointer( fireport ) )
		return;

	const auto fireport_transform = *reinterpret_cast< unity::c_transform** >( fireport + 0x10 );
	if ( !engine::mem::valid_pointer( fireport_transform ) )
		return;

	auto fireport_position = fireport_transform->position( );
	if ( fireport_position.empty( ) )
		return;

	auto bone_transform = features::aimbot.aimbot_target->bone_transform<unity::c_transform*>( get_aimbone_index( menu::vars::aim_bone ) );
	if ( !engine::mem::valid_pointer( bone_transform ) )
		return;

	auto target_pos = bone_transform->position( );
	if ( target_pos.empty( ) )
		return;

	auto aim_angle = calc_angle( fireport_position, target_pos );

	const auto movement_context = engine::local_player->movement_context( );
	if ( !engine::mem::valid_pointer( movement_context ) )
		return;

	if ( engine::get_key( ( engine::key_code )menu::vars::aim_key ) || get_mouse_button_fn( get_aimkey_index( menu::vars::mouse_aim_key ) ) )
		*reinterpret_cast< geometry::vec2_t* >( movement_context + 0x22C ) = { aim_angle.x, aim_angle.y };
}