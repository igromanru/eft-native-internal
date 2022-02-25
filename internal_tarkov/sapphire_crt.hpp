#pragma once
#include <cstdint>
#include <codecvt>
#include <intrin.h>

#undef min
#undef max
#define max( a, b ) ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#define min( a, b ) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )

namespace chloe
{
	namespace crt
	{
		namespace memory 
		{
			inline void* memcpy( void* destination, const void* source, std::size_t size )
			{
				auto data_source = static_cast< const std::uint8_t* >( source );
				auto data_destination = static_cast< std::uint8_t* >( destination );

				__movsb( data_destination, data_source, size );
				return static_cast< void* >( data_destination );
			}
		}

		namespace string
		{
			inline unsigned int wcslen( const wchar_t* str )
			{
				int counter = 0;
				if ( !str )
					return 0;
				for ( ; *str != '\0'; ++str )
					++counter;

				return counter;
			}

			inline bool wcsicmp( wchar_t* a, const wchar_t* b ) 
			{
				if ( !a || !b )
					return !a && !b;

				int ret = 0;
				wchar_t* p1 = ( wchar_t* )a;
				wchar_t* p2 = ( wchar_t* )b;
				while ( !( ret = *p1 - *p2 ) && *p2 )
					++p1, ++p2;

				return ret == 0;
			}

			inline const char* strstr( const char* input, const char* find )
			{
				while ( *( input++ ) != '\0' ) {
					const char* p, * q;
					for ( p = input, q = find; *q != '\0' && *p == *q; p++, q++ ) { }
					if ( *q == '\0' ) {
						return input;
					}
				}

				return 0;
			}

			inline int wcsicmp_insensitive( const wchar_t* cs, const wchar_t* ct )
			{
				const auto len = wcslen( cs );
				if ( len != wcslen( ct ) )
					return false;

				for ( size_t i{ 0 }; i < len; i++ )
					if ( ( cs[ i ] | L' ' ) != ( ct[ i ] | L' ' ) )
						return false;

				return true;
			}

			inline unsigned short utf8_to_utf16( char* val ) {
				std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
				std::u16string dest = convert.from_bytes( val );
				return *( unsigned short* )&dest[ 0 ];
			}

			inline bool strcmp( const char* a, const char* b ) 
			{
				if ( !a || !b )
					return !a && !b;

				int ret = 0;
				unsigned char* p1 = ( unsigned char* )a;
				unsigned char* p2 = ( unsigned char* )b;
				while ( !( ret = *p1 - *p2 ) && *p2 )
					++p1, ++p2;

				return ret == 0;
			}
		}
	}
}