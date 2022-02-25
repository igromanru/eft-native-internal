#include "weapon.hpp"
#include "menu.hpp"

auto has_gun( ) -> bool
{
	const auto hands_controller = *reinterpret_cast< std::uintptr_t* >(reinterpret_cast<std::uintptr_t>(engine::local_player) + 0x508 );
	if ( !engine::mem::valid_pointer( hands_controller ) )
		return false;

	auto item_ = *reinterpret_cast< std::uintptr_t* >( hands_controller + 0x60 );
	if ( !engine::mem::valid_pointer( item_ ) )
		return false;

	auto unk1 = *reinterpret_cast< std::uintptr_t* >( item_ + 0x90 );
	if ( !engine::mem::valid_pointer( unk1 ) )
		return false;

	auto unk2 = *reinterpret_cast< std::uintptr_t* >( unk1 + 0x20 );
	if ( !engine::mem::valid_pointer( unk2 ) )
		return false;

	auto unk3 = *reinterpret_cast< std::uintptr_t* >( unk2 + 0x38 );
	if ( !engine::mem::valid_pointer( unk3 ) )
		return false;

	auto unk4 = *reinterpret_cast< std::uintptr_t* >( unk3 + 0x40 );
	if ( !engine::mem::valid_pointer( unk4 ) )
		return false;

	return true;
}

auto features::c_weapon_mods::instant_aim( ) -> void
{
	if ( !engine::mem::valid_pointer( engine::game_world ) )
		return;

	if ( !engine::mem::valid_pointer( engine::local_player ) )
		return;

	const auto procedural_weapon_animation = *reinterpret_cast< std::uintptr_t* >(reinterpret_cast<std::uintptr_t>(engine::local_player) + 0x190 );
	if ( !engine::mem::valid_pointer( ( void* )procedural_weapon_animation ) )
		return;

	*reinterpret_cast< float* >( procedural_weapon_animation + 0x198 ) = 1000.f;
}

auto features::c_weapon_mods::weapon_removals( ) -> void
{
	if ( !engine::mem::valid_pointer( engine::game_world ) )
		return;

	if ( !engine::mem::valid_pointer( engine::local_player ) )
		return;

	if ( !has_gun( ) )
		return;

	const auto procedural_weapon_animation = *reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( engine::local_player ) + 0x190 );
	if ( !engine::mem::valid_pointer( procedural_weapon_animation ) )
		return;

	if ( menu::vars::no_spread ) {
		const auto shot_effector = *reinterpret_cast< std::uintptr_t* >( procedural_weapon_animation + 0x48 );
		if ( !engine::mem::valid_pointer( shot_effector ) )
			return;

		*reinterpret_cast< float* >( shot_effector + 0x70 ) = 0.f;
	}

	if ( menu::vars::no_sway ) {
		const auto breath_effector = *reinterpret_cast< std::uintptr_t* >( procedural_weapon_animation + 0x28 );
		if ( !engine::mem::valid_pointer( breath_effector ) )
			return;

		*reinterpret_cast< float* >( breath_effector + 0xA4 ) = 0.f;
	}

	if ( menu::vars::no_recoil ) {
		*reinterpret_cast< int* >( procedural_weapon_animation + 0x100 ) = 1;
	}
}