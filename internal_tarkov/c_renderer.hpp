#pragma once

class unity::c_renderer 
{
public:
	auto get_material( ) -> unity::c_material*
	{
		MONO_METHOD( get_material_fn, E( "UnityEngine::Renderer.get_material()" ), -1, unity::c_material * ( * )( void* ) );
		return get_material_fn( this );
	}
};