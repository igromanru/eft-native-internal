#pragma once
#include "c_player.hpp"
#include "c_world.hpp"
#include "geometry.hpp"
#include "xorstr.hpp"
#include "unity.hpp"
#include <array>
#include <cstdint>

namespace engine
{
	inline c_player* local_player;
	inline c_world* game_world;

	namespace mem
	{
		template<typename t>
		inline auto valid_pointer( t pointer ) -> bool {
			return ( pointer && pointer > ( t )0x400000 && pointer < ( t )0x7FFFFFFFFFFF );
		}

		inline auto read_chain( uint64_t base, const std::vector<uint64_t>& offsets ) -> std::uint64_t
		{
			uint64_t result = *reinterpret_cast< uint64_t* >( base + offsets.at( 0 ) );

			if ( !engine::mem::valid_pointer( ( void* )result ) )
				return {};

			for ( int i = 1; i < offsets.size( ); i++ ) {
				result = *reinterpret_cast< uint64_t* >( result + offsets.at( i ) );

				if ( !engine::mem::valid_pointer( ( void* )result ) )
					return {};
			}

			return result;
		};
	}

	class game_object {
	public:
		static auto create_game_object( std::uintptr_t ptr, managed_system::c_string name ) -> void {
			MONO_METHOD( internal_create_game_object_fn, E( "UnityEngine::GameObject.Internal_CreateGameObject()" ), -1, void( * )( std::uintptr_t, managed_system::c_string ) );
			return internal_create_game_object_fn( ptr, name );
		}

		static auto add_component( std::uintptr_t ptr, std::uintptr_t type ) -> std::uintptr_t
		{
			MONO_METHOD( add_component_fn, E( "UnityEngine::GameObject.AddComponent()" ), -1, std::uintptr_t( * )( std::uintptr_t, std::uintptr_t ) );
			return add_component_fn( ptr, type );
		}
	};

	class object
	{
	public:
		static auto dont_destroy_on_load( std::uintptr_t object ) -> void
		{
			MONO_METHOD( dont_destroy_on_load_fn, E( "UnityEngine::Object.DontDestroyOnLoad()" ), -1, void( * )( std::uintptr_t ) );
			return dont_destroy_on_load_fn( object );
		}

		static auto destroy( std::uintptr_t object ) -> void
		{
			MONO_METHOD( destroy_fn, E( "UnityEngine::Object.Destroy()" ), 1, void( * )( std::uintptr_t ) );
			return destroy_fn( object );
		}
	};

	namespace screen
	{
		inline auto transform_point( geometry::vec3_t wrld ) -> geometry::vec3_t
		{
			const auto main_camera = unity::c_camera::get_main( );
			if ( !main_camera )
				return { 0, 0, 0 };

			auto w2s_point = main_camera->world_to_screen_point( wrld );

			w2s_point.y = unity::c_screen::get_height( ) - w2s_point.y;

			if ( w2s_point.z < 0.01f )
				return { 0, 0, 0 };

			return w2s_point;
		}

		inline auto calculate_fov( geometry::vec3_t pos ) -> float
		{
			auto screen_pos = transform_point( pos );
			if ( screen_pos.x < 0 && screen_pos.y < 0 )
				return 10000;

			auto get_2d_dist = [ & ]( const geometry::vec2_t& source, const geometry::vec3_t& dest ) {
				return geometry::math::sqrtf( ( source.x - dest.x ) * ( source.x - dest.x ) + ( source.y - dest.y ) * ( source.y - dest.y ) );
			};

			return get_2d_dist( geometry::vec2_t( unity::c_screen::get_width( ) / 2.f, unity::c_screen::get_height( ) / 2.f ), screen_pos );
		}
	}
}