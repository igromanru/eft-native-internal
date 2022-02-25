#pragma once

class unity::c_material 
{
public:
	auto get_shader( ) -> unity::c_shader* {
		MONO_METHOD( get_shader_fn, E( "UnityEngine::Material.get_shader()" ), -1, unity::c_shader * ( * )( void* ) );
		return get_shader_fn( this );
	}

	auto set_shader( unity::c_shader* shader ) -> void {
		MONO_METHOD( set_shader_fn, E( "UnityEngine::Material.set_shader()" ), -1, void( * )( void*, void* ) );
		return set_shader_fn( this, shader );
	}

	auto set_int( managed_system::c_string prop, int value ) -> void {
		MONO_METHOD( set_int_fn, E( "UnityEngine::Material.SetInt()" ), -1, void( * )( void*, managed_system::c_string, int ) );
		return set_int_fn( this, prop, value );
	}

	template<typename t>
	auto set_color( managed_system::c_string prop, t color ) -> void {
		MONO_METHOD( set_color_fn, E( "UnityEngine::Material.SetColor()" ), -1, void( * )( void*, managed_system::c_string, t ) );
		return set_color_fn( this, prop, color );
	}

	auto set_texture( managed_system::c_string prop, std::uintptr_t texture ) -> void {
		MONO_METHOD( set_texture_fn, E( "UnityEngine::Material.SetTexture()" ), 2, void( * )( void*, managed_system::c_string, std::uintptr_t ) );
		return set_texture_fn( this, prop, texture );
	}
};