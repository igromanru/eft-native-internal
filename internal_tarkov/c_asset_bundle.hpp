#pragma once
#include "unity.hpp"

class unity::c_asset_bundle
{
public:
	static auto load_from_memory( std::uintptr_t data ) -> unity::c_asset_bundle* {
		MONO_METHOD( load_from_memory_fn, E( "UnityEngine::AssetBundle.LoadFromMemory()" ), 2, unity::c_asset_bundle * ( * )( std::uintptr_t, std::uint32_t ) );
		return load_from_memory_fn( data, 0 );
	}

	static auto load_from_file( managed_system::c_string path ) -> unity::c_asset_bundle* {
		MONO_METHOD( load_from_memory_fn, E( "UnityEngine::AssetBundle.LoadFromFile()" ), 3, unity::c_asset_bundle * ( * )( managed_system::c_string, std::uint32_t, std::uint64_t ) );
		return load_from_memory_fn( path, 0, 0 );
	}

	template<typename type>
	auto load_asset( managed_system::c_string name, std::uintptr_t type_object ) -> type {
		MONO_METHOD( load_asset_fn, E( "UnityEngine::AssetBundle.LoadAsset_Internal()" ), -1, void* ( * )( void*, managed_system::c_string, std::uintptr_t ) );
		return reinterpret_cast< type >( load_asset_fn( this, name, type_object ) );
	}
};