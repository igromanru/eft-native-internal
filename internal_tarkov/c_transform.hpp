#pragma once

class unity::c_transform
{
public:
	inline geometry::vec3_t position( )
	{
		auto transform_internal = *reinterpret_cast< uintptr_t* >( std::uintptr_t( this ) + 0x10 );

		auto matrices = *reinterpret_cast< uintptr_t* >( transform_internal + 0x38 );
		auto index = *reinterpret_cast< int* >( transform_internal + 0x40 );

		auto matrix_list_base = *reinterpret_cast< uintptr_t* >( matrices + 0x18 );
		auto dependency_index_table_base = *reinterpret_cast< uintptr_t* >( matrices + 0x20 );

		static auto get_dependency_index = [ & ]( uintptr_t base, int32_t index )
		{
			chloe::crt::memory::memcpy( &index, ( void* )( base + index * 4 ), sizeof( index ) );
			return index;
		};

		static auto get_matrix_blob = [ & ]( uintptr_t base, uintptr_t offs, float* blob, uint32_t size )
		{
			chloe::crt::memory::memcpy( blob, ( void* )( base + offs ), size );
		};

		int32_t index_relation = get_dependency_index( dependency_index_table_base, index );

		geometry::vec3_t ret_value;
		{
			float* base_matrix3x4 = ( float* )malloc( 64 ),
				* matrix3x4_buffer0 = ( float* )( ( uintptr_t )base_matrix3x4 + 16 ),
				* matrix3x4_buffer1 = ( float* )( ( uintptr_t )base_matrix3x4 + 32 ),
				* matrix3x4_buffer2 = ( float* )( ( uintptr_t )base_matrix3x4 + 48 );

			get_matrix_blob( matrix_list_base, index * 48, base_matrix3x4, 16 );

			__m128 xmmword_1410D1340 = { -2.f, 2.f, -2.f, 0.f };
			__m128 xmmword_1410D1350 = { 2.f, -2.f, -2.f, 0.f };
			__m128 xmmword_1410D1360 = { -2.f, -2.f, 2.f, 0.f };

			while ( index_relation >= 0 )
			{
				uint32_t matrix_relation_index = 6 * index_relation;

				get_matrix_blob( matrix_list_base, 8 * matrix_relation_index, matrix3x4_buffer2, 16 );
				__m128 v_0 = *( __m128* )matrix3x4_buffer2;

				get_matrix_blob( matrix_list_base, 8 * matrix_relation_index + 32, matrix3x4_buffer0, 16 );
				__m128 v_1 = *( __m128* )matrix3x4_buffer0;

				get_matrix_blob( matrix_list_base, 8 * matrix_relation_index + 16, matrix3x4_buffer1, 16 );
				__m128i v9 = *( __m128i* )matrix3x4_buffer1;

				__m128* v3 = ( __m128* )base_matrix3x4; // r10@1
				__m128 v10; // xmm9@2
				__m128 v11; // xmm3@2
				__m128 v12; // xmm8@2
				__m128 v13; // xmm4@2
				__m128 v14; // xmm2@2
				__m128 v15; // xmm5@2
				__m128 v16; // xmm6@2
				__m128 v17; // xmm7@2

				v10 = _mm_mul_ps( v_1, *v3 );
				v11 = _mm_castsi128_ps( _mm_shuffle_epi32( v9, 0 ) );
				v12 = _mm_castsi128_ps( _mm_shuffle_epi32( v9, 85 ) );
				v13 = _mm_castsi128_ps( _mm_shuffle_epi32( v9, -114 ) );
				v14 = _mm_castsi128_ps( _mm_shuffle_epi32( v9, -37 ) );
				v15 = _mm_castsi128_ps( _mm_shuffle_epi32( v9, -86 ) );
				v16 = _mm_castsi128_ps( _mm_shuffle_epi32( v9, 113 ) );

				v17 = _mm_add_ps(
					_mm_add_ps(
						_mm_add_ps(
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps( _mm_mul_ps( v11, ( __m128 )xmmword_1410D1350 ), v13 ),
									_mm_mul_ps( _mm_mul_ps( v12, ( __m128 )xmmword_1410D1360 ), v14 ) ),
								_mm_castsi128_ps( _mm_shuffle_epi32( _mm_castps_si128( v10 ), -86 ) ) ),
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps( _mm_mul_ps( v15, ( __m128 )xmmword_1410D1360 ), v14 ),
									_mm_mul_ps( _mm_mul_ps( v11, ( __m128 )xmmword_1410D1340 ), v16 ) ),
								_mm_castsi128_ps( _mm_shuffle_epi32( _mm_castps_si128( v10 ), 85 ) ) ) ),
						_mm_add_ps(
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps( _mm_mul_ps( v12, ( __m128 )xmmword_1410D1340 ), v16 ),
									_mm_mul_ps( _mm_mul_ps( v15, ( __m128 )xmmword_1410D1350 ), v13 ) ),
								_mm_castsi128_ps( _mm_shuffle_epi32( _mm_castps_si128( v10 ), 0 ) ) ),
							v10 ) ),
					v_0 );

				*v3 = v17;

				index_relation = get_dependency_index( dependency_index_table_base, index_relation );
			}

			ret_value = *( geometry::vec3_t* )base_matrix3x4;
			delete[ ] base_matrix3x4;
		}

		return ret_value;
	}
	/*
	inline auto position( ) -> geometry::vec3_t
	{
		geometry::vec3_t position;
		const static auto unity_base = chloe::get_sapphire_module( E( L"UnityPlayer.dll" ) ).base;
		const static auto get_position_injected = reinterpret_cast< void( __fastcall* )( void*, geometry::vec3_t& ) >( std::uintptr_t( unity_base ) + 0x9960A0 );
		get_position_injected( this, position );

		return position;
	}*/
	/*
	inline auto rotation( ) -> geometry::vec4_t
	{
		geometry::vec4_t rotation;
		const static auto unity_base = chloe::get_sapphire_module( E( L"UnityPlayer.dll" ) ).base;
		const static auto get_rotation_injected = reinterpret_cast< void( __fastcall* )( void*, geometry::vec4_t& ) >( std::uintptr_t( unity_base ) + 0x996180 );
		get_rotation_injected( this, rotation );

		return rotation;
	}

	inline auto right( ) -> geometry::vec3_t
	{
		MONO_METHOD( get_transform_right_fn, E( "UnityEngine::Transform.get_right()" ), -1, geometry::vec3_t( * )( void* ) );
		return get_transform_right_fn( this );
	}

	inline auto up( ) -> geometry::vec3_t
	{
		MONO_METHOD( get_transform_up_fn, E( "UnityEngine::Transform.get_up()" ), -1, geometry::vec3_t( * )( void* ) );
		return get_transform_up_fn( this );
	}

	inline auto set_forward( geometry::vec3_t vector ) ->void
	{
		MONO_METHOD( set_transform_forward_fn, E( "UnityEngine::Transform.set_forward()" ), -1, void( * )( void*, geometry::vec3_t ) );
		return set_transform_forward_fn( this, vector );
	}*/
};