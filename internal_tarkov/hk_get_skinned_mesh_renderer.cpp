#include "hooks.hpp"
#include "engine_render.hpp"
#include "unity.hpp"
#include "menu.hpp"

unity::c_shader* shader;

auto impl::hooks::hk_get_skinned_mesh_renderer( void* instance ) -> std::uintptr_t
{
	impl::hooks::get_skinned_mesh_renderer.toggle_hook( true );
	const auto renderer = impl::hooks::get_skinned_mesh_renderer.get_original<unity::c_renderer* ( * )( void* )>( )( instance );
	impl::hooks::get_skinned_mesh_renderer.toggle_hook( false );

	if ( renderer )//&& menu::vars::chams )
	{
		if ( !engine::gui::bundle )
			return std::uintptr_t( renderer );

		const auto material = renderer->get_material( );
		if ( !shader )
			shader = engine::gui::bundle->load_asset<unity::c_shader*>( E( L"force_filled.shader" ), mono_lib::type_object( E( "UnityEngine" ), E( "Shader" ) ) );

		if ( material && material->get_shader( ) != shader )
		{
			material->set_shader( shader );
			material->set_texture( E( L"_MainTex" ), 0 );
			material->set_int( E( L"_FresnelPowerBehind" ), 2 );
			material->set_int( E( L"_FresnelPowerVisible" ), 3 );
			material->set_color<engine::color_t>( E( L"_ColorVisible" ), { 255, 0, 0, 255 } );
			material->set_color<engine::color_t>( E( L"_ColorBehind" ), { 0, 255, 0, 255 } );
		}
	}

	return std::uintptr_t( renderer );
}