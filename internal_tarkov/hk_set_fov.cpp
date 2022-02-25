#include "hooks.hpp"
#include "menu.hpp"

auto impl::hooks::hk_set_fov( void* instance, float x, float time, bool applyFovOnCamera ) -> void
{
	x += menu::vars::camera_fov;

	impl::hooks::set_fov.toggle_hook( true );
	impl::hooks::set_fov.get_original<decltype( &impl::hooks::hk_set_fov )>( )( instance, x, time, applyFovOnCamera );
	impl::hooks::set_fov.toggle_hook( false );

	return;
}