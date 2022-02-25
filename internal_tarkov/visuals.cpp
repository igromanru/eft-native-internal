#include "visuals.hpp"
#include "aimbot.hpp"
#include "menu.hpp"

// todo: clean up this fucking mess or kill myself.

auto features::c_visuals::render_player( c_player* player ) -> void
{
	if ( !player )
		return;

	auto profile = player->profile( );

	if ( !engine::mem::valid_pointer( profile ) )
		return;

	auto info = profile->info( );

	if ( !engine::mem::valid_pointer( info ) )
		return;

	auto local_head_transform = engine::local_player->bone_transform<unity::c_transform*>( 133 );

	if ( !engine::mem::valid_pointer( local_head_transform ) )
		return;

	auto base_transform = player->bone_transform<unity::c_transform*>( 0 );

	if ( !engine::mem::valid_pointer( base_transform ) )
		return;

	auto head_transform = player->bone_transform<unity::c_transform*>( 133 );

	if ( !engine::mem::valid_pointer( head_transform ) )
		return;

	auto base_position = base_transform->position( );
	if ( base_position.empty( ) )
		return;

	auto head_position = head_transform->position( );

	if ( head_position.empty( ) )
		return;

	auto base_w2s = engine::screen::transform_point( base_position );
	auto standing_w2s = engine::screen::transform_point( base_position + geometry::vec3_t{ 0, 1.9f, 0 } );
	auto head_w2s = engine::screen::transform_point( head_position );

	float box_height = ( base_w2s.y - head_w2s.y ) * 1.2f;
	float box_width = ( base_w2s.y - standing_w2s.y ) * 0.45f;

	auto box_pos = geometry::vec2_t{ base_w2s.x - box_width * 0.5f, base_w2s.y - box_height };

	engine::color_t esp_color = { 255, 255, 255, 180 };

	const auto draw_player = [ & ]( c_player* plr )
	{
		esp_color = { 220, 80, 40, 180 };

		if ( menu::vars::player_esp_options.at( 0 ) )
		{
			engine::gui::rect( box_pos.x - 1, box_pos.y - 1, box_width + 2, box_height + 2, { 0, 0, 0, 180 } );
			engine::gui::rect( box_pos.x, box_pos.y, box_width, box_height, esp_color );
		}

		if ( menu::vars::player_esp_options.at( 1 ) )
		{
			auto name = info->name( );

			if ( !engine::mem::valid_pointer( ( void* )name ) )
				return;

			const auto half = box_width / 2.f;
			engine::gui::string( box_pos.x - 100.f, box_pos.y - 18.f, box_width + 200.f, 20.f, name, { 255, 255, 255, 255 }, true, true );
		}

		std::vector< std::pair< std::wstring, engine::color_t > > flags;

		if ( menu::vars::player_esp_flags.at( 0 ) )
		{
			auto distance = static_cast< int >( local_head_transform->position( ).distance_3d( head_transform->position( ) ) );
			auto distance_string = std::to_wstring( distance ) + std::wstring( E( L"ft" ) );
			flags.emplace_back( distance_string, engine::color_t{ 200, 200, 200, 180 } );
		}

		for ( std::size_t i = 0; i < flags.size( ); ++i )
		{
			auto& [flag, color] = flags[ i ];
			engine::gui::string( box_pos.x + box_width + 2, box_pos.y + ( 10 * i ) - 4, 300, 20, flag.c_str( ), color, false, true );
		}

		if ( menu::vars::player_esp_options.at( 2 ) )
		{
			const auto health_controller = *reinterpret_cast< uintptr_t* >( plr + 0x4F0 );

			if ( !engine::mem::valid_pointer( ( void* )health_controller ) )
				return;

			const auto health_body = *reinterpret_cast< uintptr_t* >( health_controller + 0x50 );

			if ( !engine::mem::valid_pointer( ( void* )health_body ) )
				return;

			const auto body_controller = *reinterpret_cast< uintptr_t* >( health_body + 0x18 );

			if ( !engine::mem::valid_pointer( ( void* )body_controller ) )
				return;

			float maxHealth{}, health{};

			for ( int i = 0; i <= 6; i++ )
			{
				auto bodyPart = *reinterpret_cast< uintptr_t* >( body_controller + 0x30 + ( i * 0x18 ) );
				if ( !engine::mem::valid_pointer( ( void* )bodyPart ) )
					return;

				auto healthContainer = *reinterpret_cast< std::uintptr_t* >( bodyPart + 0x10 );

				if ( !engine::mem::valid_pointer( ( void* )healthContainer ) )
					return;

				auto current_health = *reinterpret_cast< int* >( healthContainer + 0x10 );
				auto max_health = *reinterpret_cast< int* >( healthContainer + 0x14 );

				health += current_health;
				maxHealth += max_health;
			}

			float HealthBarHeight = ( health / maxHealth ) * ( box_height - 2 );
			engine::gui::filled_rect( { box_pos.x - 7, box_pos.y, 4, box_height }, { 0, 0, 0, 180 } );
			engine::gui::filled_rect( { box_pos.x - 6, box_pos.y + box_height - HealthBarHeight - 1, 2, HealthBarHeight }, esp_color );
		}

		const auto weapon = engine::mem::read_chain( std::uint64_t( plr ), { 0x190, 0x48, 0x28 } );
		if ( weapon )
		{
			const static auto get_short_name_fn = reinterpret_cast< managed_system::c_string * ( * )( std::uintptr_t ) >( mono_lib::method( E( "EFT.InventoryLogic" ), E( "Item" ), E( "get_ShortName" ), -1 ) );

			MONO_OBFUSCATED_METHOD( localize_fn, E( "Assembly-CSharp" ), E( "\\uE6AF" ), E( "Localized" ), managed_system::c_string * ( * )( managed_system::c_string*, managed_system::c_string, managed_system::c_string ) );

			const auto short_name = get_short_name_fn( weapon );

			if ( short_name )
			{
				const auto localized_name = localize_fn( short_name, nullptr, nullptr );

				if ( menu::vars::player_esp_options.at( 4 ) )
				{
					engine::gui::string( box_pos.x - 100.f, box_pos.y + box_height + 2.f, box_width + 200.f, 20.f, localized_name->buffer, { 255, 255, 255, 255 }, true, true );
				}
			}

			const static auto get_current_magazine_fn = reinterpret_cast< std::uintptr_t( * )( std::uintptr_t ) >( mono_lib::method( E( "EFT.InventoryLogic" ), E( "Weapon" ), E( "GetCurrentMagazine" ), -1 ) );

			MONO_OBFUSCATED_METHOD( max_count_fn, E( "Assembly-CSharp" ), E( "\\uE8EC" ), E( "get_MaxCount" ), int( * )( std::uintptr_t ) );
			MONO_OBFUSCATED_METHOD( count_fn, E( "Assembly-CSharp" ), E( "\\uE8EC" ), E( "get_Count" ), int( * )( std::uintptr_t ) );

			if ( const auto magazine = get_current_magazine_fn( weapon ) )
			{
				float max_ammo = max_count_fn( magazine );
				float current_ammo = count_fn( magazine );
				float ammo_bar_width = float( current_ammo / max_ammo ) * ( box_width - 2.f );

				if ( menu::vars::player_esp_options.at( 3 ) )
				{
					engine::gui::filled_rect( { box_pos.x, box_pos.y + box_height - 1 + 5, box_width, 4 }, { 0, 0, 0, 180 } );
					engine::gui::filled_rect( { box_pos.x + 1, box_pos.y + box_height + 5, ammo_bar_width, 2 }, esp_color );
				}
			}
		}

		return;
	};

	const auto draw_player_scav = [ & ]( c_player* plr )
	{
		esp_color = { 124, 184, 252, 180 };

		if ( menu::vars::player_scav_options.at( 0 ) )
		{
			engine::gui::rect( box_pos.x - 1, box_pos.y - 1, box_width + 2, box_height + 2, { 0, 0, 0, 180 } );
			engine::gui::rect( box_pos.x, box_pos.y, box_width, box_height, esp_color );
		}

		if ( menu::vars::player_scav_options.at( 1 ) )
		{
			auto name = info->name( );

			if ( !engine::mem::valid_pointer( ( void* )name ) )
				return;

			const auto half = box_width / 2.f;
			engine::gui::string( box_pos.x - 100.f, box_pos.y - 18.f, box_width + 200.f, 20.f, name, { 255, 255, 255, 255 }, true, true );
		}

		std::vector< std::pair< std::wstring, engine::color_t > > flags;

		if ( menu::vars::player_scav_flags.at( 1 ) )
		{
			auto distance = static_cast< int >( local_head_transform->position( ).distance_3d( head_transform->position( ) ) );
			auto distance_string = std::to_wstring( distance ) + std::wstring( L"ft" );
			flags.emplace_back( distance_string, engine::color_t{ 200, 200, 200, 180 } );
		}

		for ( std::size_t i = 0; i < flags.size( ); ++i )
		{
			auto& [flag, color] = flags[ i ];
			engine::gui::string( box_pos.x + box_width + 2, box_pos.y + ( 10 * i ) - 4, 300, 20, flag.c_str( ), color, false, true );
		}

		if ( menu::vars::player_scav_options.at( 2 ) )
		{
			const auto health_controller = *reinterpret_cast< uintptr_t* >( plr + 0x4F0 );

			if ( !engine::mem::valid_pointer( ( void* )health_controller ) )
				return;

			const auto health_body = *reinterpret_cast< uintptr_t* >( health_controller + 0x50 );

			if ( !engine::mem::valid_pointer( ( void* )health_body ) )
				return;

			const auto body_controller = *reinterpret_cast< uintptr_t* >( health_body + 0x18 );

			if ( !engine::mem::valid_pointer( ( void* )body_controller ) )
				return;

			float maxHealth{}, health{};

			for ( int i = 0; i <= 6; i++ )
			{
				auto bodyPart = *reinterpret_cast< uintptr_t* >( body_controller + 0x30 + ( i * 0x18 ) );
				if ( !engine::mem::valid_pointer( ( void* )bodyPart ) )
					return; // *not a mistake* if any body part is invalid then gtfo

				auto healthContainer = *reinterpret_cast< std::uintptr_t* >( bodyPart + 0x10 );

				if ( !engine::mem::valid_pointer( ( void* )healthContainer ) )
					return;

				auto current_health = *reinterpret_cast< int* >( healthContainer + 0x10 );
				auto max_health = *reinterpret_cast< int* >( healthContainer + 0x14 );

				health += current_health;
				maxHealth += max_health;
			}

			float HealthBarHeight = ( health / maxHealth ) * ( box_height - 2 );
			engine::gui::filled_rect( { box_pos.x - 7, box_pos.y, 4, box_height }, { 0, 0, 0, 180 } );
			engine::gui::filled_rect( { box_pos.x - 6, box_pos.y + box_height - HealthBarHeight - 1, 2, HealthBarHeight }, esp_color );
		}

		const auto weapon = engine::mem::read_chain( std::uint64_t( plr ), { 0x190, 0x48, 0x28 } );
		if ( weapon )
		{
			const static auto get_short_name_fn = reinterpret_cast< managed_system::c_string * ( * )( std::uintptr_t ) >( mono_lib::method( E( "EFT.InventoryLogic" ), E( "Item" ), E( "get_ShortName" ), -1 ) );
			MONO_OBFUSCATED_METHOD( localize_fn, E( "Assembly-CSharp" ), E( "\\uE6AF" ), E( "Localized" ), managed_system::c_string * ( * )( managed_system::c_string*, managed_system::c_string, managed_system::c_string ) );

			const auto short_name = get_short_name_fn( weapon );

			if ( short_name )
			{
				const auto localized_name = localize_fn( short_name, nullptr, nullptr );

				if ( menu::vars::player_scav_options.at( 4 ) )
				{
					engine::gui::string( box_pos.x - 100.f, box_pos.y + box_height + 2.f, box_width + 200.f, 20.f, localized_name->buffer, { 255, 255, 255, 180 }, true, true );
				}
			}

			const static auto get_current_magazine_fn = reinterpret_cast< std::uintptr_t( * )( std::uintptr_t ) >( mono_lib::method( E( "EFT.InventoryLogic" ), E( "Weapon" ), E( "GetCurrentMagazine" ), -1 ) );

			MONO_OBFUSCATED_METHOD( max_count_fn, E( "Assembly-CSharp" ), E( "\\uE8EC" ), E( "get_MaxCount" ), int( * )( std::uintptr_t ) );
			MONO_OBFUSCATED_METHOD( count_fn, E( "Assembly-CSharp" ), E( "\\uE8EC" ), E( "get_Count" ), int( * )( std::uintptr_t ) );

			if ( const auto magazine = get_current_magazine_fn( weapon ) )
			{
				float max_ammo = max_count_fn( magazine );
				float current_ammo = count_fn( magazine );
				float ammo_bar_width = float( current_ammo / max_ammo ) * ( box_width - 2.f );

				if ( menu::vars::player_scav_options.at( 3 ) )
				{
					engine::gui::filled_rect( { box_pos.x, box_pos.y + box_height - 1 + 5, box_width, 4 }, { 0, 0, 0, 180 } );
					engine::gui::filled_rect( { box_pos.x + 1, box_pos.y + box_height + 5, ammo_bar_width, 2 }, esp_color );
				}
			}
		}

		return;
	};

	const auto draw_scav = [ & ]( c_player* plr )
	{
		esp_color = { 169, 124, 252, 180 };

		if ( menu::vars::scav_esp_options.at( 0 ) )
		{
			engine::gui::rect( box_pos.x - 1, box_pos.y - 1, box_width + 2, box_height + 2, { 0, 0, 0, 180 } );
			engine::gui::rect( box_pos.x, box_pos.y, box_width, box_height, esp_color );
		}

		if ( menu::vars::scav_esp_options.at( 1 ) )
		{
			auto name = info->name( );

			if ( !engine::mem::valid_pointer( name ) )
				return;

			auto size = engine::gui::calculate_text_size( name );

			float text_w = size.x, text_h = size.y;

			auto mid = box_pos.x + box_width / 2.f;
			auto _x = mid - text_w / 2 - 5.f;
			auto _y = box_pos.y - 14.f;

			engine::gui::filled_rect( { box_pos.x + box_width / 2 - size.x / 2.f - 2, box_pos.y - 18.f + 1.f, size.x + 4.f, 14 }, { 42, 42, 42, 180 } );

			engine::gui::string( box_pos.x + box_width / 2 - size.x / 2.f + 1.f, box_pos.y - 18.f - 1.f, 300.f, 20, name, { 0, 0, 0, 255 }, false, false );
			engine::gui::string( box_pos.x + box_width / 2 - size.x / 2.f, box_pos.y - 18.f - 2.f, 300.f, 20, name, { 255, 255, 255, 255 }, false, false );
		}

		std::vector< std::pair< std::wstring, engine::color_t > > flags;

		if ( menu::vars::scav_esp_flags.at( 0 ) )
		{
			auto distance = static_cast< int >( local_head_transform->position( ).distance_3d( head_transform->position( ) ) );
			auto distance_string = std::to_wstring( distance ) + std::wstring( L"M" );
			flags.emplace_back( distance_string, engine::color_t{ 255, 255, 255, 255 } );
		}

		for ( std::size_t i = 0; i < flags.size( ); ++i )
		{
			auto& [flag, color] = flags[ i ];

			auto flag_str = E( L"<size=9>" ) + flag + E( L"</size>" );

			engine::gui::string( box_pos.x + box_width + 3, box_pos.y + ( 10 * i ) - 4 + 1, 300, 20, flag_str.c_str( ), engine::color_t( 0, 0, 0, 255 ), false, false );
			engine::gui::string( box_pos.x + box_width + 2, box_pos.y + ( 10 * i ) - 4, 300, 20, flag_str.c_str( ), color, false, false );
		}

		if ( menu::vars::scav_esp_options.at( 2 ) )
		{
			float health{ }, maxHealth{ };
			plr->get_health_and_max_health( health, maxHealth );


			float HealthBarHeight = ( health / maxHealth ) * ( box_height - 2 );
			engine::gui::filled_rect( { box_pos.x - 7, box_pos.y, 4, box_height }, { 0, 0, 0, 180 } );
			engine::gui::filled_rect( { box_pos.x - 6, box_pos.y + box_height - HealthBarHeight - 1, 2, HealthBarHeight }, esp_color );
		}

		const auto weapon = engine::mem::read_chain( std::uint64_t( plr ), { 0x190, 0x48, 0x28 } );
		if ( weapon )
		{
			const static auto get_short_name_fn = reinterpret_cast< managed_system::c_string * ( * )( std::uintptr_t ) >( mono_lib::method( E( "EFT.InventoryLogic" ), E( "Item" ), E( "get_ShortName" ), -1 ) );

			MONO_OBFUSCATED_METHOD( localize_fn, E( "Assembly-CSharp" ), E( "\\uE6AF" ), E( "Localized" ), managed_system::c_string * ( * )( managed_system::c_string*, managed_system::c_string, managed_system::c_string ) );

			const auto short_name = get_short_name_fn( weapon );

			const auto localized_name = localize_fn( short_name, nullptr, nullptr );

			auto buffer = localized_name->buffer;


			const static auto get_current_magazine_fn = reinterpret_cast< std::uintptr_t( * )( std::uintptr_t ) >( mono_lib::method( E( "EFT.InventoryLogic" ), E( "Weapon" ), E( "GetCurrentMagazine" ), -1 ) );

			MONO_OBFUSCATED_METHOD( max_count_fn, E( "Assembly-CSharp" ), E( "\\uE8EC" ), E( "get_MaxCount" ), int( * )( std::uintptr_t ) );
			MONO_OBFUSCATED_METHOD( count_fn, E( "Assembly-CSharp" ), E( "\\uE8EC" ), E( "get_Count" ), int( * )( std::uintptr_t ) );

			if ( const auto magazine = get_current_magazine_fn( weapon ) )
			{
				float max_ammo = max_count_fn( magazine );
				float current_ammo = count_fn( magazine );
				float ammo_bar_width = float( current_ammo / max_ammo ) * ( box_width - 2.f );

				auto str = std::wstring( buffer ) + E( L" (" ) + std::to_wstring( static_cast< int >( current_ammo ) ) + E( L")" );

				auto size = engine::gui::calculate_text_size( str.c_str( ) );

				engine::gui::filled_rect( { box_pos.x + box_width / 2 - size.x / 2.f - 2, box_pos.y + box_height + 3.f, size.x + 4.f, 14 }, { 42, 42, 42, 180 } );

				engine::gui::string( box_pos.x + box_width / 2 - size.x / 2.f + 1.f, box_pos.y + box_height - 1.f + 2.f, 300.f, 20, str.c_str( ), { 0, 0, 0, 255 }, false, false );
				engine::gui::string( box_pos.x + box_width / 2 - size.x / 2.f, box_pos.y + box_height, 300.f, 20, str.c_str( ), { 255, 255, 255, 255 }, false, false );
			}
		}

		return;
	};


	if ( base_w2s.empty( ) || head_w2s.empty( ) || standing_w2s.empty( ) )
		return;

	if ( info->registration_date( ) > 0 )
	{
		auto player_side = info->player_side( );

		if ( player_side != 4 )
		{
			draw_player( player );
		}
		else
		{
			draw_player_scav( player );
		}
	}
	else
	{
		draw_scav( player );
	}
}

auto resolve_fire_mode( int mode ) -> std::wstring
{
	switch ( mode )
	{
	case 0:
		return E( L"fullauto" );
	case 1:
		return E( L"single" );
	case 2:
		return E( L"doublet" );
	case 3:
		return E( L"burst" );
	case 4:
		return E( L"doubleaction" );
	}
}

auto features::c_visuals::local_ammo( ) -> void 
{
	if ( !engine::mem::valid_pointer( engine::local_player ) )
		return;

	const auto weapon = engine::mem::read_chain( std::uint64_t( engine::local_player ), { 0x190, 0x48, 0x28 } );
	if ( weapon )
	{
		const static auto get_current_magazine_fn = reinterpret_cast< std::uintptr_t( * )( std::uintptr_t ) >( mono_lib::method( E( "EFT.InventoryLogic" ), E( "Weapon" ), E( "GetCurrentMagazine" ), -1 ) );
		const static auto get_selected_fire_mode_fn = reinterpret_cast< int( * )( std::uintptr_t ) >( mono_lib::method( E( "EFT.InventoryLogic" ), E( "Weapon" ), E( "get_SelectedFireMode" ), -1 ) );
		const static auto get_chamber_ammo_count_fn = reinterpret_cast< int( * )( std::uintptr_t ) >( mono_lib::method( E( "EFT.InventoryLogic" ), E( "Weapon" ), E( "get_ChamberAmmoCount" ), -1 ) );

		MONO_OBFUSCATED_METHOD( max_count_fn, E( "Assembly-CSharp" ), E( "\\uE8EC" ), E( "get_MaxCount" ), int( * )( std::uintptr_t ) );
		MONO_OBFUSCATED_METHOD( count_fn, E( "Assembly-CSharp" ), E( "\\uE8EC" ), E( "get_Count" ), int( * )( std::uintptr_t ) );

		if ( const auto magazine = get_current_magazine_fn( weapon ) )
		{
			float max_ammo = max_count_fn( magazine );
			float current_ammo = count_fn( magazine ) + get_chamber_ammo_count_fn( weapon );

			menu::vars::ammo_count = std::wstring( E( L"ammo count: " ) ) + std::to_wstring( static_cast< int >( current_ammo ) ) + E( L"/" ) + std::to_wstring( static_cast< int >( max_ammo ) );

			auto fire_mode = get_selected_fire_mode_fn( weapon );

			auto fire_mode_str = resolve_fire_mode( fire_mode );

			menu::vars::fire_mode = std::wstring( E( L"firemode: " ) + fire_mode_str );
		}
	}

}

auto get_open_status( std::uintptr_t extract ) -> bool
{
	if ( !engine::mem::valid_pointer( ( void* )extract ) )
		return false;

	const auto status = *reinterpret_cast<int*>( extract + 0xA8 );

	if ( status == 1 || status == 2 )
		return false;

	return true;
}

auto features::c_visuals::render_extract( std::uintptr_t extract ) -> void
{
	if ( !menu::vars::extract_esp )
		return;

	if ( !engine::mem::valid_pointer( ( void* )extract ) )
		return;

	if ( !engine::mem::valid_pointer( ( void* )engine::local_player ) )
		return;

	const auto unk1 = *reinterpret_cast< std::uintptr_t* >( extract + 0x58 );
	if ( !engine::mem::valid_pointer( ( void* )unk1 ) )
		return;

	const auto name_obj = *reinterpret_cast< std::uintptr_t* >( unk1 + 0x10 );
	if ( !engine::mem::valid_pointer( ( void* )name_obj ) )
		return;

	const auto name = reinterpret_cast< managed_system::c_string* >( name_obj )->buffer;

	if ( !get_open_status( extract ) )
		return;

	const auto tfm_unk1 = *reinterpret_cast< std::uintptr_t* >( extract + 0x10 );
	if ( !engine::mem::valid_pointer( ( void* )tfm_unk1 ) )
		return;

	const auto tfm_unk2 = *reinterpret_cast< std::uintptr_t* >( tfm_unk1 + 0x30 );
	if ( !engine::mem::valid_pointer( ( void* )tfm_unk2 ) )
		return;

	const auto tfm_unk3 = *reinterpret_cast< std::uintptr_t* >( tfm_unk2 + 0x30 );
	if ( !engine::mem::valid_pointer( ( void* )tfm_unk3 ) )
		return;

	const auto tfm_unk4 = *reinterpret_cast< std::uintptr_t* >( tfm_unk3 + 0x8 );
	if ( !engine::mem::valid_pointer( ( void* )tfm_unk4 ) )
		return;

	const auto tfm_unk5 = *reinterpret_cast< unity::c_transform** >( tfm_unk4 + 0x28 );
	if ( !engine::mem::valid_pointer( ( void* )tfm_unk5 ) )
		return;

	const auto position = tfm_unk5->position( );

	const auto distance = static_cast< int >( engine::local_player->bone_transform<unity::c_transform*>( 133 )->position( ).distance_3d( position ) );
	const auto distance_str = std::to_wstring( distance ) + std::wstring( E( L"ft" ) );

	auto w2s_pos = engine::screen::transform_point( position );
	if ( w2s_pos.empty( ) )
		return;

	engine::gui::string( w2s_pos.x, w2s_pos.y, 300, 20, name, { 0, 255, 0, 180 }, false, true );

	if ( menu::vars::extract_esp_flags.at( 0 ) )
		engine::gui::string( w2s_pos.x, w2s_pos.y + 12.f, 300, 20, distance_str.c_str( ), { 0, 255, 0, 180 }, false, true );
}