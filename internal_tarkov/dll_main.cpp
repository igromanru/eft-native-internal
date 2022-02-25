#include "mono_lib.hpp"
#include "hooks.hpp"
#include "managed_system.hpp"
#include "notifications.hpp"
#include "script_manager.hpp"
#include "menu.hpp"

#include "js_renderer.hpp"
#include "js_cheat.hpp"

auto __stdcall DllMain( void*, std::uint32_t call_reason, void* ) -> bool
{
	if ( call_reason != 1 )
		return false;

	mono_lib::init( );
	{
		MONO_OBFUSCATED_METHOD( set_fov, E( "Assembly-CSharp" ), E( "\\uE77B" ), E( "SetFov" ), void* );
		MONO_OBFUSCATED_METHOD( initialize_shot, E( "Assembly-CSharp" ), E( "\\uEA64" ), E( "\\uE004" ), void* );
		
		const auto game_object = mono_lib::object_new( mono_lib::get_root_domain( ), mono_lib::find_class( E( "GameObject" ), E( "UnityEngine" ) ) );
		engine::game_object::create_game_object( game_object, E( L"" ) );
		const auto component = engine::game_object::add_component( game_object, mono_lib::type_object( E( "" ), E( "GUIController" ) ) );
		engine::object::dont_destroy_on_load( game_object );

		impl::hooks::gui_controller_start.setup( E( "GUIController.Start()" ), &impl::hooks::hk_gui_controller_start );
		impl::hooks::gui_controller_update.setup( E( "GUIController.Update()" ), &impl::hooks::hk_gui_controller_update );
		impl::hooks::gui_controller_ongui.setup( E( "GUIController.OnGUI()" ), &impl::hooks::hk_gui_controller_ongui );

		impl::hooks::set_fov.setup( std::uintptr_t( set_fov ), &impl::hooks::hk_set_fov );
		impl::hooks::initialize_shot.setup( std::uintptr_t( initialize_shot ), &impl::hooks::hk_initialize_shot );	
		impl::hooks::apply_shot.setup( E( "EFT::Player.ApplyShot()" ), &impl::hooks::hk_apply_shot );
		
		impl::hooks::preloader_ui_update_fn.setup( E( "EFT.UI" ), E( "PreloaderUI" ), E( "Update" ), &impl::hooks::hk_preloader_ui_update_fn );
		impl::hooks::overall_screen_show.setup( E( "EFT.UI" ), E( "OverallScreen" ), E( "Show" ), &impl::hooks::hk_overall_screen_show );
	}

	return true;
}