#include "hooks.hpp"

auto impl::hooks::hk_preloader_ui_update_fn( void* instance ) -> void
{
	const static auto set_session_id = reinterpret_cast< void( * )( void*, managed_system::c_string ) >( mono_lib::method( E( "EFT.UI" ), E( "PreloaderUI" ), E( "SetSessionId" ), 1 ) );
	set_session_id( instance, E( L"by chloe." ) );

	impl::hooks::preloader_ui_update_fn.toggle_hook( true );
	impl::hooks::preloader_ui_update_fn.get_original<decltype( &impl::hooks::hk_preloader_ui_update_fn )>( )( instance );
	impl::hooks::preloader_ui_update_fn.toggle_hook( false );

	return;
}