#include "hooks.hpp"
#include "aimbot.hpp"
#include "menu.hpp"

auto impl::hooks::hk_initialize_shot( void* instance, void* ammo, int fragmentIndex, int seed, geometry::vec3_t origin, geometry::vec3_t direction, float initialSpeed, float speed, float bulletMassKg, float bulletDiameterM, float damage, float penetrationPower, float penetrationChance, float ricochetChance, float fragmentationChance, float deviationChance, int minFragmentsCount, int maxFragmentsCount, void* defaultBallisticCollider, void* randoms, float ballisticCoefficient, c_player* player, void* weapon, int fireIndex, void* parent ) -> void {

	if ( engine::mem::valid_pointer( engine::local_player ) && player == engine::local_player )
	{
		if ( menu::vars::aim_type == 1 && features::aimbot.aimbot_target )
		{
			auto target_pos = features::aimbot.aimbot_target->bone_transform<unity::c_transform*>( features::aimbot.get_aimbone_index( menu::vars::aim_bone ) )->position( );
			if ( !target_pos.empty( ) )
				direction = ( target_pos - origin ).normalized( );
		}
	}

	impl::hooks::initialize_shot.get_original<decltype( &impl::hooks::hk_initialize_shot )>( )( instance, ammo, fragmentIndex, seed, origin, direction, initialSpeed, speed, bulletMassKg, bulletDiameterM, damage, penetrationPower, penetrationChance, ricochetChance, fragmentationChance, deviationChance, minFragmentsCount, maxFragmentsCount, defaultBallisticCollider, randoms, ballisticCoefficient, player, weapon, fireIndex, parent );
}