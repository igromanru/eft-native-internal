#pragma once
#include "sapphire_importer.hpp"
#include "xorstr.hpp"
#include <string>
#include <vector>

#define CREATE_TYPE( name, args ) using mono_##name = args; inline mono_##name name;
#define ASSIGN_TYPE( name ) name = SAPPHIRE_TYPE( E( L"mono-2.0-bdwgc.dll" ), mono_##name );

#define MONO_OBFUSCATED_METHOD( fn_name, assembly, klass_name, method, args ) \
	static auto fn_name = reinterpret_cast< args >( mono_lib::get_obfuscated_method( mono_lib::get_obfuscated_class( assembly, klass_name ), method ) )

#define MONO_METHOD( fn_name, name, arg_count, args ) \
	static auto fn_name = reinterpret_cast< args >( mono_lib::method( name, arg_count ) )

#define MONO_METHOD_ARGS( fn_name, name, arg_count, arg_name, args ) \
	static auto fn_name = reinterpret_cast< args >( mono_lib::arg_method( name, arg_count, arg_name ) )

namespace mono_lib
{
	struct sanitized_method_t { std::string ns, kl, mt; };

	CREATE_TYPE( get_root_domain, std::uintptr_t( * )( ) );
	CREATE_TYPE( class_get_methods, std::uintptr_t( * )( std::uintptr_t, std::uintptr_t* ) );
	CREATE_TYPE( method_get_param_count, int ( * )( std::uintptr_t ) );
	CREATE_TYPE( assembly_get_image, std::uintptr_t( * )( std::uintptr_t ) );
	CREATE_TYPE( object_new, std::uintptr_t( * )( std::uintptr_t, std::uintptr_t ) );
	CREATE_TYPE( class_from_name, std::uintptr_t( * )( std::uintptr_t, const char*, const char* ) );
	CREATE_TYPE( field_static_get_value, std::uintptr_t( * )( std::uintptr_t, std::uintptr_t* ) );
	CREATE_TYPE( class_get_fields, std::uintptr_t( * )( std::uintptr_t, std::uintptr_t* ) );
	CREATE_TYPE( field_get_offset, std::uintptr_t( * )( std::uintptr_t ) );
	CREATE_TYPE( class_get_type, std::uintptr_t( * )( std::uintptr_t ) );
	CREATE_TYPE( type_get_object, std::uintptr_t( * )( std::uintptr_t, std::uintptr_t ) );
	CREATE_TYPE( compile_method, std::uintptr_t( * )( std::uintptr_t ) );
	CREATE_TYPE( assembly_foreach, void( * )( void*, void* ) );
	CREATE_TYPE( class_get_method_from_name, std::uintptr_t( * )( std::uintptr_t, const char* name, int ) );
	CREATE_TYPE( method_signature, std::uintptr_t( * )( std::uintptr_t ) );
	CREATE_TYPE( signature_get_param_count, int( * )( std::uintptr_t ) );
	CREATE_TYPE( method_get_param_names, void( * )( std::uintptr_t, const char** ) );
	CREATE_TYPE( array_new, std::uintptr_t( * )( std::uintptr_t, std::uintptr_t, int ) );
	CREATE_TYPE( array_addr_with_size, char* ( * )( std::uintptr_t, int, std::uintptr_t ) );
	CREATE_TYPE( get_byte_class, std::uintptr_t( * )( ) );
	CREATE_TYPE( image_get_table_info, std::uintptr_t( * )( std::uintptr_t, int ) );
	CREATE_TYPE( table_info_get_rows, int( * )( std::uintptr_t ) );
	CREATE_TYPE( class_get, std::uintptr_t( * )( std::uintptr_t, std::uint32_t ) );
	CREATE_TYPE( class_get_name, const char* ( * )( std::uintptr_t ) );
	CREATE_TYPE( class_get_namespace, const char* ( * )( std::uintptr_t ) );
	CREATE_TYPE( domain_assembly_open, std::uintptr_t( * )( std::uintptr_t, const char* ) );
	CREATE_TYPE( method_get_name, const char* ( * )( std::uintptr_t ) );
	CREATE_TYPE( thread_attach, void( * )( std::uintptr_t ) );
	CREATE_TYPE( jit_info_table_find, std::uintptr_t( * )( std::uintptr_t, std::uintptr_t ) );

	inline void init( ) {
		ASSIGN_TYPE( get_root_domain );
		ASSIGN_TYPE( class_get_methods );
		ASSIGN_TYPE( method_get_param_count );
		ASSIGN_TYPE( assembly_get_image );
		ASSIGN_TYPE( object_new );
		ASSIGN_TYPE( class_from_name );
		ASSIGN_TYPE( field_static_get_value );
		ASSIGN_TYPE( class_get_fields );
		ASSIGN_TYPE( field_get_offset );
		ASSIGN_TYPE( class_get_type );
		ASSIGN_TYPE( type_get_object );
		ASSIGN_TYPE( compile_method );
		ASSIGN_TYPE( assembly_foreach );
		ASSIGN_TYPE( class_get_method_from_name );
		ASSIGN_TYPE( method_signature );
		ASSIGN_TYPE( signature_get_param_count );
		ASSIGN_TYPE( method_get_param_names );
		ASSIGN_TYPE( array_new );
		ASSIGN_TYPE( array_addr_with_size );
		ASSIGN_TYPE( get_byte_class );
		ASSIGN_TYPE( image_get_table_info );
		ASSIGN_TYPE( table_info_get_rows );
		ASSIGN_TYPE( class_get );
		ASSIGN_TYPE( class_get_name );
		ASSIGN_TYPE( class_get_namespace );
		ASSIGN_TYPE( domain_assembly_open );
		ASSIGN_TYPE( method_get_name );
		ASSIGN_TYPE( thread_attach );
		ASSIGN_TYPE( jit_info_table_find );

		thread_attach( get_root_domain( ) );
	}

	inline void _cdecl enumerator( void* callback, std::vector<void*>* vec ) // pushes back every assembly loaded, even ones outside of root domain
	{
		vec->push_back( callback );
	}

	inline auto get_obfuscated_class( const char* assembly_name, const char* klass_name ) -> const std::uintptr_t
	{
		const auto assembly = domain_assembly_open( get_root_domain( ), assembly_name );
		if ( !assembly )
			return { };

		const auto img = assembly_get_image( std::uintptr_t( assembly ) );
		if ( !img )
			return { };

		auto type_def = image_get_table_info( img, 2 );
		if ( !type_def )
			return { };

		auto type_def_count = table_info_get_rows( type_def );
		if ( type_def_count <= 0 )
			return { };

		for ( int i = 0; i < type_def_count; i++ )
		{
			auto this_class = class_get( img, 0x02000000 | i + 1 );
			if ( !this_class )
				continue;

			auto name = class_get_name( this_class );
			auto klass_namespace = class_get_namespace( this_class );

			if ( ( std::uint8_t )name[ 0 ] == 0xEE ) {
				char name_buff[ 32 ];
				sprintf_s( name_buff, 32, E( "\\u%04X" ), chloe::crt::string::utf8_to_utf16( ( char* )name ) );
				name = name_buff;
			}

			if ( chloe::crt::string::strcmp( ( char* )name, ( char* )klass_name ) ) {
				return this_class;
			}
		}

		return { };
	}

	inline auto find_class( const char* klass_name, const char* name_space = "" ) -> const std::uintptr_t
	{
		const auto domain = get_root_domain( );

		std::vector<void*> v;
		assembly_foreach( mono_lib::enumerator, &v );

		for ( auto assembly : v )
		{
			if ( !assembly )
				continue;

			const auto img = assembly_get_image( std::uintptr_t( assembly ) );
			if ( !img )
				continue;

			const auto klass = class_from_name( img, name_space, klass_name );
			if ( !klass )
				continue;

			return klass;
		}

		return {};
	}

	inline auto type_object( const char* name_space, const char* name ) -> const std::uintptr_t
	{
		auto klass = find_class( name, name_space );
		return type_get_object( get_root_domain( ), class_get_type( klass ) );
	}

	// im sorry this is messy but its a lot cleaner in the end.
	inline auto sanitize_method_name( const std::string name ) -> const sanitized_method_t
	{
		std::string ns, kl, method_name;

		if ( auto ns_break = name.find( E( "::" ) ) )
			ns = name.substr( 0, ns_break );

		if ( name.find( E( "::" ) ) == std::string::npos )
			ns = E( "" );

		if ( name.find( E( "::" ) ) ) {
			kl = name.substr( name.find( E( "::" ), 2 ) + 2 );
			kl = kl.substr( 0, kl.find( E( "." ), 0 ) );
		}

		if ( name.find( E( "::" ) ) == std::string::npos )
			kl = name.substr( 0, name.find( E( "." ), 0 ) );

		method_name = name.substr( name.find( E( "." ), 0 ) + 1 );

		if ( method_name.find( E( "()" ) ) )
			method_name.erase( method_name.find( E( "()" ) ), 2 );

		return { ns, kl, method_name };
	}

	inline auto method( const std::string name, int argument_number = -1 ) -> const std::uintptr_t {
		std::uintptr_t iter{};

		const auto st = sanitize_method_name( name );
		const auto klass = find_class( st.kl.c_str( ), st.ns.c_str( ) );
		if ( !klass )
			return{ };

		const auto method = class_get_method_from_name( klass, st.mt.c_str( ), argument_number );
		if ( !method )
			return { };

		return compile_method( method );
	}

	inline auto get_obfuscated_method( const std::uintptr_t klass, const char* method_name ) -> const std::uintptr_t
	{
		std::uintptr_t iter{};

		while ( auto method = class_get_methods( klass, &iter ) )
		{
			auto name = method_get_name( method );

			if ( ( std::uint8_t )name[ 0 ] == 0xEE ) {
				char name_buff[ 32 ];
				sprintf_s( name_buff, 32, E( "\\u%04X" ), chloe::crt::string::utf8_to_utf16( ( char* )name ) );
				name = name_buff;
			}

			if ( chloe::crt::string::strcmp( name, method_name ) ) {
				return compile_method( method );
			}
		}

		return {};
	}

	inline auto method( const std::uintptr_t klass, const char* method_name, int argument_number = -1 ) -> const std::uintptr_t
	{
		const auto method = class_get_method_from_name( klass, method_name, argument_number );
		if ( !method )
			return { };

		return compile_method( method );
	}

	inline auto method( const char* ns, const char* klass_name, const char* name, int argument_number = -1 ) -> const std::uintptr_t {
		std::uintptr_t iter{};

		const auto klass = find_class( klass_name, ns );
		if ( !klass )
			return{ };

		const auto method = class_get_method_from_name( klass, name, argument_number );
		if ( !method )
			return { };

		return compile_method( method );
	}

	inline auto arg_method( const std::string name, int argument_number = -1, const char* arg_name = "" ) -> const std::uintptr_t {
		std::uintptr_t iter{};

		const auto st = sanitize_method_name( name );
		const auto klass = find_class( st.kl.c_str( ), st.ns.c_str( ) );
		if ( !klass )
			return{ };

		while ( const auto method = class_get_methods( klass, &iter ) )
		{
			const auto signature = method_signature( method );
			if ( !signature )
				continue;

			auto param_count = signature_get_param_count( signature );
			if ( !param_count || param_count < argument_number || param_count > argument_number )
				continue;

			const char* _names[ 128 ] = { 0 };

			method_get_param_names( method, _names );

			for ( int i = 0; i < param_count; i++ )
			{
				if ( _names[ i ] )
				{
					if ( chloe::crt::string::strcmp( _names[ i ], arg_name ) )
						return compile_method( method );
				}
			}

		}

		return { };
	}
}