#pragma once
#include "sapphire_crt.hpp"

namespace managed_system
{
	template<typename type>
	class c_list {
	private:
		char pad_0x10[ 0x10 ];
		uintptr_t list_base;
		uint32_t count;
	public:
		int get_count( ) {
			if ( !this || !list_base ) 
				return 0;

			return int( this->count );
		}

		type get( uint32_t index ) {
			if ( !this || !list_base ) 
				return type( );
			return *reinterpret_cast< type* >( this->list_base + 0x20 + index * 0x8 );
		}
	};

	class c_string
	{
	public:
		char zpad[ 0x10 ]{ };
		int size{ };
		wchar_t buffer[ 128 + 1 ];
	public:
		c_string( const wchar_t* st )
		{
			size = min( chloe::crt::string::wcslen( st ), 128 );
			for ( size_t idx{ 0 }; idx < size; idx++ )
				buffer[ idx ] = st[ idx ];

			buffer[ size ] = 0;
		}
	};
}