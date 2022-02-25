	#pragma once
#include "mono_lib.hpp"

namespace define
{
	// rewritten dumbhook, this is sigged. u will need to change it! - chloe

	class jit_hook_c
	{
	private:
		std::uintptr_t m_original{ 0 };
		std::uint8_t* old_instructions{ 0 };
		std::uint8_t* new_instructions{ 0 };
	public:
		auto hook_fn( std::uintptr_t target_function, void* our_function ) -> const bool
		{
			unsigned char* _ptr = ( unsigned char* )target_function;

			_ptr[ 0 ] = 0x48;
			_ptr[ 1 ] = 0xB8;
			*( void** )( _ptr + 2 ) = our_function;
			_ptr[ 10 ] = 0xFF;
			_ptr[ 11 ] = 0xE0;

			return true;
		}

		auto setup( std::string name, void* our_function, int arg_count = -1 ) -> bool
		{
			auto jit_func_ptr = mono_lib::method( name, arg_count );
			if ( !jit_func_ptr )
				return false;

			constexpr static auto hook_size = 12;

			old_instructions = new std::uint8_t[ hook_size ];

			chloe::crt::memory::memcpy( old_instructions, reinterpret_cast< void* >( jit_func_ptr ), hook_size );
			this->m_original = jit_func_ptr;

			if ( !hook_fn( jit_func_ptr, our_function ) )
			{
				old_instructions = nullptr;
				return false;
			}

			new_instructions = new std::uint8_t[ hook_size ];
			chloe::crt::memory::memcpy( new_instructions, reinterpret_cast< void* >( jit_func_ptr ), hook_size );

			return true;
		}

		auto setup( std::uintptr_t target_function, void* our_function ) -> bool
		{
			constexpr static auto hook_size = 12;

			old_instructions = new std::uint8_t[ hook_size ];

			chloe::crt::memory::memcpy( old_instructions, reinterpret_cast< void* >( target_function ), hook_size );
			this->m_original = target_function;

			if ( !hook_fn( target_function, our_function ) )
			{
				old_instructions = nullptr;
				return false;
			}

			new_instructions = new std::uint8_t[ hook_size ];
			chloe::crt::memory::memcpy( new_instructions, reinterpret_cast< void* >( target_function ), hook_size );

			return true;
		}

		auto setup( const char* name_space, const char* klass_name, const char* method_name, void* our_function, int arg_count = -1 ) -> bool
		{
			auto jit_func_ptr = mono_lib::method( name_space, klass_name, method_name, arg_count );
			if ( !jit_func_ptr )
				return false;

			constexpr static auto hook_size = 12;

			old_instructions = new std::uint8_t[ hook_size ];

			chloe::crt::memory::memcpy( old_instructions, reinterpret_cast< void* >( jit_func_ptr ), hook_size );
			this->m_original = jit_func_ptr;

			if ( !hook_fn( jit_func_ptr, our_function ) )
			{
				old_instructions = nullptr;
				return false;
			}

			new_instructions = new std::uint8_t[ hook_size ];
			chloe::crt::memory::memcpy( new_instructions, reinterpret_cast< void* >( jit_func_ptr ), hook_size );

			return true;
		}

		bool toggle_hook( bool unhook )
		{
			if ( !this->m_original || !new_instructions )
				return false;

			constexpr static auto hook_size = 12;

			auto jit_func_ptr = reinterpret_cast< std::uint8_t* >( this->m_original );
			auto instructions = unhook ? this->old_instructions : this->new_instructions;

			chloe::crt::memory::memcpy( jit_func_ptr, instructions, hook_size );

			return true;
		}

		template< typename type >
		auto get_original( ) -> type
		{
			return reinterpret_cast< type >( m_original );
		}
	};
}