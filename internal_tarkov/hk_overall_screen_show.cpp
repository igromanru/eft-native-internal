#include "hooks.hpp"
#include "menu.hpp"

enum e_member_category : int
{
	default_ = 0,
	developer = 1,
	unique_id = 2,
	trader = 4,
	group = 8,
	system_ = 16,
	chat_moderator = 32,
	chat_moderator_with_permanent_ban = 64,
	unit_test = 128,
	sherpa = 256,
	emissary = 512
};

auto impl::hooks::hk_overall_screen_show( void* instance, c_profile* profile, void* inventoryController )  -> void {

	auto info = profile->info( );

	auto category = e_member_category::default_;

	switch ( menu::vars::profile_category )
	{
	case 0:
		category = e_member_category::default_;
		break;
	case 1:
		category = e_member_category::unique_id;
		break;
	case 2:
		category = e_member_category::developer;
		break;
	case 3:
		category = e_member_category::sherpa;
		break;
	case 4:
		category = e_member_category::emissary;
		break;
	}

	*reinterpret_cast< int* >( info + 0x70 ) = category;

	impl::hooks::overall_screen_show.toggle_hook( true );
	impl::hooks::overall_screen_show.get_original<decltype( &impl::hooks::hk_overall_screen_show )>( )( instance, profile, inventoryController );
	impl::hooks::overall_screen_show.toggle_hook( false );
}