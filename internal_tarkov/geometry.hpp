#pragma once
#define M_PI (float)3.14159265358979323846f
#define DEG2RAD( x )  ( (float)(x) * (float)(M_PI / 180.f) )
#define RAD2DEG( x )  ( (float)(x) * (float)(180.f / M_PI) )

namespace geometry
{
	namespace math 
	{
		inline static float sqrtf( float number )
		{
			long i;
			float x2, y;
			const float threehalfs = 1.5F;

			x2 = number * 0.5F;
			y = number;
			i = *( long* )&y;
			i = 0x5f3759df - ( i >> 1 );
			y = *( float* )&i;
			y = y * ( threehalfs - ( x2 * y * y ) );
			y = y * ( threehalfs - ( x2 * y * y ) );

			return 1 / y;
		}
	}

	struct mat4x4_t
	{
		mat4x4_t( )
			: m{ { 0, 0, 0, 0 },
				 { 0, 0, 0, 0 },
				 { 0, 0, 0, 0 },
				 { 0, 0, 0, 0 } }
		{}

		mat4x4_t( const mat4x4_t& ) = default;

		float* operator[]( size_t i ) { return m[ i ]; }
		const float* operator[]( size_t i ) const { return m[ i ]; }

		auto transpose( ) -> mat4x4_t {
			mat4x4_t m;

			for ( int i = 0; i < 4; i++ )
				for ( int j = 0; j < 4; j++ )
					m.m[ i ][ j ] = this->m[ j ][ i ];

			return m;
		}

		union {
			struct {
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[ 4 ][ 4 ];
		};
	};

	class vec2_t
	{
	public:
		float x;
		float y;

		vec2_t( )
		{}

		vec2_t( float x, float y )
			: x( x ), y( y )
		{}
	};

	class vec3_t
	{
	public:
		float x;
		float y;
		float z;

		vec3_t( )
		{}

		vec3_t( float x, float y, float z )
			: x( x ), y( y ), z( z )
		{}

		vec3_t operator+( const vec3_t& vector ) const
		{
			return vec3_t( x + vector.x, y + vector.y, z + vector.z );
		}

		vec3_t operator-( const vec3_t& vector ) const
		{
			return vec3_t( x - vector.x, y - vector.y, z - vector.z );
		}

		vec3_t operator * ( float number ) const
		{
			return vec3_t( x * number, y * number, z * number );
		}

		vec3_t operator/( float number ) const
		{
			return vec3_t( x / number, y / number, z / number );
		}

		vec3_t& operator/=( float number ) 
		{
			x /= number;
			y /= number;
			z /= number;

			return *this;
		}

		auto empty( ) -> bool { return x == 0 && y == 0 && z == 0; }

		auto length( ) -> float { return math::sqrtf( ( x * x ) + ( y * y ) + ( z * z ) ); }

		vec3_t normalized( )
		{
			float len = length( );
			return vec3_t( x / len, y / len, z / len );
		}

		auto distance_3d( const vec3_t& vector ) -> float { return ( *this - vector ).length( ); }
		auto dot( const vec3_t& vector ) -> float { return x * vector.x + y * vector.y + z * vector.z; }
	};

	class vec4_t
	{
	public:
		float x;
		float y;
		float z;
		float w;

		vec4_t( ) {}
		vec4_t( float x, float y, float z, float w ) : x( x ), y( y ), z( z ), w( w ) {}

		auto dot( const vec4_t& vector ) -> float { return x * vector.x + y * vector.y + z * vector.z + w * vector.w; }
		auto distance( const vec4_t& vector ) -> float { return math::sqrtf( ( x - vector.x ) * ( x - vector.x ) + ( y - vector.y ) * ( y - vector.y ) + ( z - vector.z ) * ( z - vector.z ) + ( w - vector.w ) * ( w - vector.w ) ); }

		bool operator==( const vec4_t& vector ) const
		{
			return x == vector.x && y == vector.y && z == vector.z && w == vector.w;
		}

		bool operator!=( const vec4_t& vector ) const
		{
			return x != vector.x || y != vector.y || z != vector.z || w != vector.w;
		}

		vec4_t operator+( const vec4_t& vector ) const
		{
			return vec4_t( x + vector.x, y + vector.y, z + vector.z, w + vector.w );
		}

		vec4_t operator-( const vec4_t& vector ) const
		{
			return vec4_t( x - vector.x, y - vector.y, z - vector.z, w - vector.w );
		}

		vec4_t operator-( ) const
		{
			return vec4_t( -x, -y, -z, -w );
		}

		vec4_t operator*( float number ) const
		{
			return vec4_t( x * number, y * number, z * number, w * number );
		}

		vec4_t operator/( float number ) const
		{
			return vec4_t( x / number, y / number, z / number, w / number );
		}

		vec4_t& operator+=( const vec4_t& vector )
		{
			x += vector.x;
			y += vector.y;
			z += vector.z;
			w += vector.w;
			return *this;
		}

		vec4_t& operator-=( const vec4_t& vector )
		{
			x -= vector.x;
			y -= vector.y;
			z -= vector.z;
			w -= vector.w;
			return *this;
		}

		vec4_t& operator*=( float number )
		{
			x *= number;
			y *= number;
			z *= number;
			w *= number;
			return *this;
		}

		vec4_t& operator/=( float number )
		{
			x /= number;
			y /= number;
			z /= number;
			w /= number;
			return *this;
		}
	};
}