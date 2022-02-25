#pragma once
#include "mono_lib.hpp"
#include "managed_system.hpp"
#include "bundle_bytes.hpp"
#include "sdk.hpp"
#include "unity.hpp"

namespace engine
{
	class color_t
	{
	public:
		float r, g, b, a;
		color_t( float r, float g, float b, float a ) : r( r / 255 ), g( g / 255 ), b( b / 255 ), a( a / 255 ) { }
		color_t( float r, float g, float b ) : r( r ), g( g ), b( b ), a( 1 ) { }
		color_t( ) : r( 1 ), g( 1 ), b( 1 ), a( 1 ) { }

		color_t normalized( ) {

			r /= 255.f;
			g /= 255.f;
			b /= 255.f;
			a /= 255.f;

			return *this;
		}

		static color_t from_hsv( float hue, float saturation, float value, float alpha = 255 ) {
			color_t out_clr = { 0,0,0, 255 };
			if ( saturation == 0.0f )
			{
				// gray
				out_clr.r = out_clr.g = out_clr.b = 0;
				return out_clr;
			}

			hue = fmodf( hue, 1.0f ) / ( 60.0f / 360.0f );
			int   i = ( int )hue;
			float f = hue - ( float )i;
			float p = value * ( 1.0f - saturation );
			float q = value * ( 1.0f - saturation * f );
			float t = value * ( 1.0f - saturation * ( 1.0f - f ) );

			switch ( i )
			{
			case 0: out_clr.r = value; out_clr.g = t; out_clr.b = p; break;
			case 1: out_clr.r = q; out_clr.g = value; out_clr.b = p; break;
			case 2: out_clr.r = p; out_clr.g = value; out_clr.b = t; break;
			case 3: out_clr.r = p; out_clr.g = q; out_clr.b = value; break;
			case 4: out_clr.r = t; out_clr.g = p; out_clr.b = value; break;
			case 5: default: out_clr.r = value; out_clr.g = p; out_clr.b = q; break;
			}

			out_clr.a = 1.f;

			return out_clr;
		}

		static color_t from_hsb( float hue, float saturation, float brightness ) {
			float h = hue == 1.0f ? 0 : hue * 6.0f;
			float f = h - ( int )h;
			float p = brightness * ( 1.0f - saturation );
			float q = brightness * ( 1.0f - saturation * f );
			float t = brightness * ( 1.0f - ( saturation * ( 1.0f - f ) ) );

			if ( h < 1 ) {
				return color_t( ( brightness * 255 ), ( t * 255 ), ( p * 255 ) );
			}
			else if ( h < 2 ) {
				return color_t( ( q * 255 ), ( brightness * 255 ), ( p * 255 ) );
			}
			else if ( h < 3 ) {
				return color_t( ( p * 255 ), ( brightness * 255 ), ( t * 255 ) );
			}
			else if ( h < 4 ) {
				return color_t( ( p * 255 ), ( q * 255 ), ( brightness * 255 ) );
			}
			else if ( h < 5 ) {
				return color_t( ( t * 255 ), ( p * 255 ), ( brightness * 255 ) );
			}
			else {
				return color_t( ( brightness * 255 ), ( p * 255 ), ( q * 255 ) );
			}
		}

		static geometry::vec3_t col_to_hsv( color_t a )
		{
			float red, grn, blu;
			red = ( float )a.r / 255.f;
			grn = ( float )a.g / 255.f;
			blu = ( float )a.b / 255.f;
			float hue, sat, val;
			float x, f, i;

			x = min( min( red, grn ), blu );
			val = max( max( red, grn ), blu );
			if ( x == val )
			{
				hue = 0;
				sat = 0;
			}
			else
			{
				f = ( red == x ) ? grn - blu : ( ( grn == x ) ? blu - red : red - grn );
				i = ( red == x ) ? 3 : ( ( grn == x ) ? 5 : 1 );
				hue = fmod( ( i - f / ( val - x ) ) * 60, 360 );
				sat = ( ( val - x ) / val );
			}

			return { hue, sat, val };
		}

		static geometry::vec3_t to_hsv( color_t col ) {
#define SWAP_VARS(type, x, y)  type tmp = x; x = y; y = tmp;
			float K = 0.f;
			if ( col.g < col.b ) {
				SWAP_VARS( float, col.g, col.b );
				K = -1.f;
			}
			if ( col.r < col.g ) {
				SWAP_VARS( float, col.r, col.g );
				K = -2.f / 6.f - K;
			}

			geometry::vec3_t out;
			const float chroma = col.r - ( col.g < col.b ? col.g : col.b );
			out.x = fabsf( K + ( col.g - col.b ) / ( 6.f * chroma + 1e-20f ) );
			out.y = chroma / ( col.r + 1e-20f );
			out.z = col.r;

			return out;
		};
	};

	enum class event_type : int
	{
		mouse_down = 0,
		mouse_up = 1,
		mouse_drag = 3,
		key_down = 4,
		key_up = 5,
		repaint = 7
	};

	class rect_t
	{
	public:
		float x, y, w, h;
		rect_t( float _x, float _y, float _w, float _h ) : x( _x ), y( _y ), w( _w ), h( _h ) { }
		rect_t( ) : x( 0 ), y( 0 ), w( 0 ), h( 0 ) { }
	};


	class c_events
	{
	public:
		static auto get_current_event( ) -> c_events* {
			MONO_METHOD( get_current_event_fn, E( "UnityEngine::Event.get_current()" ), 0, engine::c_events * ( * )( ) );
			return get_current_event_fn( );
		}

		auto type( ) -> engine::event_type {
			MONO_METHOD( get_type_fn, E( "UnityEngine::Event.get_type()" ), 0, engine::event_type( * )( void* ) );
			return get_type_fn( this );
		}
	};

	enum class key_code // todo; clean this mess up. maybe put both of these in enums.hpp
	{
		Backspace = 8, Delete = 127, Tab = 9, Clear = 12,
		Return = 13, Pause = 19, Escape = 27, Space = 32,
		Keypad0 = 256, Keypad1 = 257, Keypad2 = 258, Keypad3 = 259,
		Keypad4 = 260, Keypad5 = 261, Keypad6 = 262, Keypad7 = 263,
		Keypad8 = 264, Keypad9 = 265, KeypadPeriod = 266, KeypadDivide = 267,
		KeypadMultiply = 268, KeypadMinus = 269, KeypadPlus = 270, KeypadEnter = 271,
		KeypadEquals = 272, UpArrow = 273, DownArrow = 274, RightArrow = 275,
		LeftArrow = 276, Insert = 277, Home = 278, End = 279,
		PageUp = 280, PageDown = 281, F1 = 282, F2 = 283,
		F3 = 284, F4 = 285, F5 = 286, F6 = 287,
		F7 = 288, F8 = 289, F9 = 290, F10 = 291,
		F11 = 292, F12 = 293, F13 = 294, F14 = 295,
		F15 = 296, Alpha0 = 48, Alpha1 = 49, Alpha2 = 50,
		Alpha3 = 51, Alpha4 = 52, Alpha5 = 53, Alpha6 = 54,
		Alpha7 = 55, Alpha8 = 56, Alpha9 = 57, Exclaim = 33,
		DoubleQuote = 34, Hash = 35, Dollar = 36, Percent = 37,
		Ampersand = 38, Quote = 39, LeftParen = 40, RightParen = 41,
		Asterisk = 42, Plus = 43, Comma = 44, Minus = 45,
		Period = 46, Slash = 47, Colon = 58, Semicolon = 59,
		Less = 60, Equals = 61, Greater = 62, Question = 63,
		At = 64, LeftBracket = 91, Backslash = 92, RightBracket = 93,
		Caret = 94, Underscore = 95, BackQuote = 96, A = 97,
		B = 98, C = 99, D = 100, E = 101,
		F = 102, G = 103, H = 104, I = 105,
		J = 106, K = 107, L = 108, M = 109,
		N = 110, O = 111, P = 112, Q = 113,
		R = 114, S = 115, T = 116, U = 117,
		V = 118, W = 119, X = 120, Y = 121,
		Z = 122, LeftCurlyBracket = 123, Pipe = 124, RightCurlyBracket = 125, Tilde = 126,
		Numlock = 300, CapsLock = 301, ScrollLock = 302, RightShift = 303,
		LeftShift = 304, RightControl = 305, LeftControl = 306, RightAlt = 307,
		LeftAlt = 308, LeftCommand = 310, LeftApple = 310, LeftWindows = 311,
		RightCommand = 309, RightApple = 309, RightWindows = 312, AltGr = 313,
		Help = 315, Pr = 316, SysReq = 317, Break = 318,
		Menu = 319, Mouse0 = 323, Mouse1 = 324, Mouse2 = 325,
		Mouse3 = 326, Mouse4 = 327, Mouse5 = 328, Mouse6 = 329
	};

	inline auto get_key( key_code key ) -> bool
	{
		MONO_METHOD( get_key_fn, E( "UnityEngine::Input.GetKeyInt()" ), 1, bool( * )( key_code ) );
		return get_key_fn( key );
	}

	namespace gui // represents UnityEngine.GUI class
	{
		inline std::uintptr_t label_style, gui_skin, white_texture;
		inline unity::c_asset_bundle* bundle;

		inline auto setup( ) -> void
		{
			MONO_METHOD( get_skin_fn, E( "UnityEngine::GUI.get_skin()" ), 0, std::uintptr_t( * )( ) );
			MONO_METHOD( get_white_texture_fn, E( "UnityEngine::Texture2D.get_whiteTexture()" ), 0, std::uintptr_t( * )( ) );

			gui_skin = get_skin_fn( );
			if ( !engine::mem::valid_pointer( ( void* )gui_skin ) )
				return;

			label_style = *reinterpret_cast< std::uintptr_t* >( gui_skin + 0x38 );
			white_texture = get_white_texture_fn( );
			if ( !engine::mem::valid_pointer( ( void* )white_texture ) )
				return;

			if ( !bundle )
			{
				std::uintptr_t data = mono_lib::array_new( mono_lib::get_root_domain( ), mono_lib::get_byte_class( ), sizeof( bundle_data ) );

				for ( auto i = 0; i < sizeof( bundle_data ); i++ )
				{
					auto __p = reinterpret_cast< std::uint8_t* >( mono_lib::array_addr_with_size( data, sizeof( std::uint8_t ), i ) );
					*__p = bundle_data[ i ];
				}

				bundle = unity::c_asset_bundle::load_from_memory( data );
			}

			MONO_METHOD( set_font_size_fn, E( "UnityEngine::GUIStyle.set_fontSize()" ), -1, void( * )( std::uintptr_t, int ) );
			MONO_METHOD( set_color_fn, E( "UnityEngine::GUI.set_color()" ), -1, void( * )( engine::color_t ) );
			MONO_METHOD( set_alignment_fn, E( "UnityEngine::GUIStyle.set_alignment()" ), -1, void( * )( std::uintptr_t, int ) );
			MONO_METHOD( set_font_style_fn, E( "UnityEngine::GUIStyle.set_fontStyle()" ), -1, void( * )( std::uintptr_t, int ) );

			const auto font = bundle->load_asset<std::uintptr_t>( E( L"tahoma.ttf" ), mono_lib::type_object( E( "UnityEngine" ), E( "Font" ) ) ); 
			*reinterpret_cast< std::uintptr_t* >( gui_skin + 0x18 ) = font;

			set_font_size_fn( label_style, 11 );
			set_alignment_fn( label_style, 0 );
			set_color_fn( { 1, 1, 1, 1 } );
		}

		inline auto filled_rect( engine::rect_t pos, engine::color_t clr ) -> void
		{
			if ( !engine::mem::valid_pointer( ( void* )white_texture ) )
				return;

			MONO_METHOD( set_color_fn, E( "UnityEngine::GUI.set_color()" ), -1, void( * )( engine::color_t ) );
			MONO_METHOD( draw_texture_fn, E( "UnityEngine::GUI.DrawTexture()" ), 2, void ( * )( engine::rect_t, std::uintptr_t ) );

			set_color_fn( clr );
			draw_texture_fn( pos, white_texture );
		}

		inline auto calculate_text_size( managed_system::c_string text ) -> geometry::vec2_t {
			if ( !engine::mem::valid_pointer( ( void* )label_style ) )
				return {};

			MONO_METHOD( create_temp_content_fn, E( "UnityEngine::GUIContent.Temp()" ), -1, std::uintptr_t( * )( managed_system::c_string* ) );

			const static auto unity_base = chloe::get_sapphire_module( E( L"UnityPlayer.dll" ) ).base;
			const static auto calc_size_fn = reinterpret_cast< void( __fastcall* )( std::uintptr_t, std::uintptr_t, geometry::vec2_t& ) >( std::uintptr_t( unity_base ) + 0x3B6E40 );

			const auto content = create_temp_content_fn( &text );

			geometry::vec2_t size;
			calc_size_fn( label_style, content, size );

			return size;
		}

		inline auto string_with_background( float x, float y, float w, float h, managed_system::c_string text, engine::color_t clr, bool drop_shadow ) -> void
		{
			if ( !engine::mem::valid_pointer( ( void* )label_style ) )
				return;

			MONO_METHOD( create_temp_content_fn, E( "UnityEngine::GUIContent.Temp()" ), -1, std::uintptr_t( * )( managed_system::c_string* ) );
			MONO_METHOD( set_color_fn, E( "UnityEngine::GUI.set_color()" ), 1, void( * )( engine::color_t ) );
			MONO_METHOD( set_alignment_fn, E( "UnityEngine::GUIStyle.set_alignment()" ), 1, void( * )( std::uintptr_t, int ) );

			MONO_METHOD_ARGS( label_fn, E( "UnityEngine::GUI.Label()" ), 3, E( "content" ), void( * )( engine::rect_t, std::uintptr_t, std::uintptr_t ) );

			const static auto unity_base = chloe::get_sapphire_module( E( L"UnityPlayer.dll" ) ).base;
			const static auto calc_size_fn = reinterpret_cast< void( __fastcall* )( std::uintptr_t, std::uintptr_t, geometry::vec2_t& ) >( std::uintptr_t( unity_base ) + 0x3B6E40 );

			const auto content = create_temp_content_fn( &text );

			geometry::vec2_t size;
			calc_size_fn( label_style, content, size );

			set_alignment_fn( label_style, 0 );

			filled_rect( { x - 2, y, size.x + 5, 16 }, { 17, 17, 17, clr.a * 255 } );

			if ( drop_shadow )
			{
				set_color_fn( { 0, 0, 0, ( clr.a * 255 ) } );
				label_fn( { x + 2, y - 1, w, h }, content, label_style );
			}

			set_color_fn( clr );
			label_fn( { x + 1, y - 2, w, h }, content, label_style );
		}

		inline auto string( float x, float y, float w, float h, managed_system::c_string text, engine::color_t clr, bool centered, bool outlined ) -> void
		{
			if ( !engine::mem::valid_pointer( label_style ) )
				return;

			MONO_METHOD( create_temp_content_fn, E( "UnityEngine::GUIContent.Temp()" ), -1, std::uintptr_t( * )( managed_system::c_string* ) );
			MONO_METHOD( set_color_fn, E( "UnityEngine::GUI.set_color()" ), 1, void( * )( engine::color_t ) );
			MONO_METHOD( set_alignment_fn, E( "UnityEngine::GUIStyle.set_alignment()" ), 1, void( * )( std::uintptr_t, int ) );

			MONO_METHOD_ARGS( label_fn, E( "UnityEngine::GUI.Label()" ), 3, E( "content" ), void( * )( engine::rect_t, std::uintptr_t, std::uintptr_t ) );

			const auto content = create_temp_content_fn( &text );

			if ( centered )
				set_alignment_fn( label_style, 4 );
			else
				set_alignment_fn( label_style, 0 );

			if ( outlined )
			{
				set_color_fn( { 0, 0, 0, ( clr.a * 255 ) } );
				label_fn( { x, y + 1, w, h }, content, label_style );
				label_fn( { x, y - 1, w, h }, content, label_style );
				label_fn( { x - 1, y, w, h }, content, label_style );
				label_fn( { x + 1, y, w, h }, content, label_style );
			}

			set_color_fn( clr );
			label_fn( { x, y, w, h }, content, label_style );
		}

		inline auto rect( float x, float y, float w, float h, engine::color_t clr ) -> void
		{
			if ( !engine::mem::valid_pointer( ( void* )white_texture ) )
				return;

			MONO_METHOD( set_color_fn, E( "UnityEngine::GUI.set_color()" ), -1, void( * )( engine::color_t ) );
			MONO_METHOD( draw_texture_fn, E( "UnityEngine::GUI.DrawTexture()" ), 2, void ( * )( engine::rect_t, std::uintptr_t ) );

			set_color_fn( clr );
			draw_texture_fn( { x, y, 1, h }, white_texture );
			draw_texture_fn( { x + w, y + 1, 1, h }, white_texture );
			draw_texture_fn( { x, y, w + 1, 1 }, white_texture );
			draw_texture_fn( { x, y + h, w, 1 }, white_texture );
		}
	}

	namespace gl
	{
		inline unity::c_material* draw_material;

		enum gl_primitive_mode : int
		{
			lines = 1,
			line_strip,
			triangles = 4,
			triangle_strip,
			quads = 7
		};

		inline auto setup( ) -> void
		{
			MONO_METHOD( dont_destroy_on_load_fn, E( "UnityEngine::Object.DontDestroyOnLoad()" ), -1, void( * )( void* ) );
			MONO_METHOD( find_shader_fn, E( "UnityEngine::Shader.Find()" ), -1, std::uintptr_t( * )( managed_system::c_string ) );
			MONO_METHOD( create_with_shader_fn, E( "UnityEngine::Material.CreateWithShader()" ), 2, void( * )( void*, std::uintptr_t ) );

			engine::gl::draw_material = *reinterpret_cast< unity::c_material** >( mono_lib::object_new( mono_lib::get_root_domain( ), mono_lib::find_class( E( "Material" ), E( "UnityEngine" ) ) ) );
			create_with_shader_fn( engine::gl::draw_material, find_shader_fn( E( L"Hidden/Internal-Colored" ) ) );

			draw_material->set_int( E( L"_SrcBlend" ), 5 );
			draw_material->set_int( E( L"_DstBlend" ), 10 );
			draw_material->set_int( E( L"_Cull" ), 0 );
			draw_material->set_int( E( L"_ZWrite" ), 0 );
		}

		inline auto gl_start( int mode ) -> void
		{
			if ( !engine::gl::draw_material )
				return;

			MONO_METHOD( set_pass_fn, E( "UnityEngine::Material.SetPass()" ), 1, bool( * )( void*, int ) );

			if ( set_pass_fn )
			{
				set_pass_fn( draw_material, 0 );

				const static auto push_fn = reinterpret_cast< void( * )( void ) >( mono_lib::method( E( "UnityEngine::GL.PushMatrix()" ), -1 ) );
				if ( !engine::mem::valid_pointer( push_fn ) )
					return;

				const static auto begin_fn = reinterpret_cast< void( * )( int ) >( mono_lib::method( E( "UnityEngine::GL.Begin()" ), 1 ) );
				if ( !engine::mem::valid_pointer( begin_fn ) )
					return;

				push_fn( );
				begin_fn( mode );
			}
		}

		inline auto gl_end( ) -> void
		{
			const static  auto end_fn = reinterpret_cast< void( * )( void ) >( mono_lib::method( E( "UnityEngine::GL.End()" ), 0 ) );
			if ( !engine::mem::valid_pointer( end_fn ) )
				return;

			const static auto pop_fn = reinterpret_cast< void( * )( void ) >( mono_lib::method( E( "UnityEngine::GL.PopMatrix()" ), 0 ) );
			if ( !engine::mem::valid_pointer( pop_fn ) )
				return;

			end_fn( );
			pop_fn( );
		}

		inline auto circle( float x, float y, float radius, engine::color_t col ) -> void
		{
			MONO_METHOD( sin_fn, E( "UnityEngine::Mathf.Sin()" ), 1, float( * )( float ) );
			MONO_METHOD( cos_fn, E( "UnityEngine::Mathf.Cos()" ), 1, float( * )( float ) );

			gl_start( 1 );

			const static auto color_fn = reinterpret_cast< void( * )( engine::color_t ) > ( mono_lib::method( E( "UnityEngine::GL.Color()" ), 1 ) );
			if ( !engine::mem::valid_pointer( color_fn ) )
				return;

			const static auto vertex_fn = reinterpret_cast< void( * )( float, float, float ) >( mono_lib::method( E( "UnityEngine::GL.Vertex3()" ), -1 ) );
			if ( !engine::mem::valid_pointer( vertex_fn ) )
				return;

			color_fn( col );

			for ( float num = 0.f; num < 6.28318548f; num += 0.05f )
			{
				vertex_fn( cos_fn( num ) * radius + x, sin_fn( num ) * radius + y, 0 );
				vertex_fn( cos_fn( num + 0.05f ) * radius + x, sin_fn( num + 0.05f ) * radius + y, 0 );
			}

			gl_end( );
		}

		inline auto in_world_line( geometry::vec3_t origin, geometry::vec3_t end, engine::color_t col ) -> void
		{
			if ( !engine::gl::draw_material )
				return;

			const static auto push_fn = reinterpret_cast< void( * )( void ) >( mono_lib::method( E( "UnityEngine::GL.PushMatrix()" ), -1 ) );
			const static auto begin_fn = reinterpret_cast< void( * )( int ) >( mono_lib::method( E( "UnityEngine::GL.Begin()" ), 1 ) );
			const static auto load_proj_matrix = reinterpret_cast< void( * )( geometry::mat4x4_t ) >( mono_lib::method( E( "UnityEngine::GL.LoadProjectionMatrix()" ), -1 ) );
			const static auto modelview = reinterpret_cast< void( * )( geometry::mat4x4_t ) >( mono_lib::method( E( "UnityEngine::GL.set_modelview()" ), -1 ) );

			MONO_METHOD( set_pass_fn, E( "UnityEngine::Material.SetPass()" ), 1, bool( * )( void*, int ) );

			set_pass_fn( draw_material, 0 );
			push_fn( );

			const auto main_camera = unity::c_camera::get_main( );

			load_proj_matrix( main_camera->projection_matrix( ) );
			modelview( main_camera->world_to_camera_matrix( ) );

			begin_fn( gl_primitive_mode::lines );

			const static auto color_fn = reinterpret_cast< void( * )( engine::color_t ) > ( mono_lib::method( E( "UnityEngine::GL.Color()" ), 1 ) );
			if ( !engine::mem::valid_pointer( color_fn ) )
				return;

			const static auto vertex_fn = reinterpret_cast< void( * )( float, float, float ) >( mono_lib::method( E( "UnityEngine::GL.Vertex3()" ), -1 ) );
			if ( !engine::mem::valid_pointer( vertex_fn ) )
				return;

			color_fn( col );
			vertex_fn( origin.x, origin.y, origin.z );
			vertex_fn( end.x, end.y, end.z );

			gl_end( );
		}

		inline auto line( float x, float y, float x1, float y1, engine::color_t col ) -> void
		{
			gl_start( gl_primitive_mode::lines );

			const static auto color_fn = reinterpret_cast< void( * )( engine::color_t ) > ( mono_lib::method( E( "UnityEngine::GL.Color()" ), 1 ) );
			if ( !engine::mem::valid_pointer( color_fn ) )
				return;

			const static auto vertex_fn = reinterpret_cast< void( * )( float, float, float ) >( mono_lib::method( E( "UnityEngine::GL.Vertex3()" ), -1 ) );
			if ( !engine::mem::valid_pointer( vertex_fn ) )
				return;

			color_fn( col );
			vertex_fn( x, y, 0 );
			vertex_fn( x1, y1, 0 );

			gl_end( );
		}

		inline auto gradient_filled_rect( float x, float y, float w, float h, engine::color_t col, engine::color_t  col2 ) -> void
		{
			gl_start( gl_primitive_mode::quads );

			const static auto color_fn = reinterpret_cast< void( * )( engine::color_t ) > ( mono_lib::method( E( "UnityEngine::GL.Color()" ), 1 ) );
			if ( !engine::mem::valid_pointer( color_fn ) )
				return;

			const static auto vertex_fn = reinterpret_cast< void( * )( float, float, float ) >( mono_lib::method( E( "UnityEngine::GL.Vertex3()" ), -1 ) );
			if ( !engine::mem::valid_pointer( vertex_fn ) )
				return;

			color_fn( col );
			vertex_fn( x, y, 0 );
			vertex_fn( x + w, y, 0 );
			color_fn( col2 );
			vertex_fn( x + w, y + h, 0 );
			vertex_fn( x, y + h, 0 );
			color_fn( col );
			vertex_fn( x, y, 0 );

			gl_end( );
		}

		inline auto filled_rect( float x, float y, float w, float h, engine::color_t col ) -> void
		{
			gl_start( gl_primitive_mode::quads );

			const static auto color_fn = reinterpret_cast< void( * )( engine::color_t ) > ( mono_lib::method( E( "UnityEngine::GL.Color()" ), 1 ) );
			if ( !engine::mem::valid_pointer( color_fn ) )
				return;

			const static auto vertex_fn = reinterpret_cast< void( * )( float, float, float ) >( mono_lib::method( E( "UnityEngine::GL.Vertex3()" ), -1 ) );
			if ( !engine::mem::valid_pointer( vertex_fn ) )
				return;

			color_fn( col );
			vertex_fn( x, y, 0 );
			vertex_fn( x + w, y, 0 );
			vertex_fn( x + w, y + h, 0 );
			vertex_fn( x, y + h, 0 );
			vertex_fn( x, y, 0 );

			gl_end( );
		}

		inline auto gradient_horizontal_filled_rect( float x, float y, float w, float h, engine::color_t col, engine::color_t  col2 ) -> void
		{
			gl_start( gl_primitive_mode::quads );

			const static auto color_fn = reinterpret_cast< void( * )( engine::color_t ) > ( mono_lib::method( E( "UnityEngine::GL.Color()" ), 1 ) );
			if ( !engine::mem::valid_pointer( color_fn ) )
				return;

			const static auto vertex_fn = reinterpret_cast< void( * )( float, float, float ) >( mono_lib::method( E( "UnityEngine::GL.Vertex3()" ), -1 ) );
			if ( !engine::mem::valid_pointer( vertex_fn ) )
				return;

			color_fn( col );
			vertex_fn( x, y, 0 );
			color_fn( col2 );
			vertex_fn( x + w, y, 0 );
			vertex_fn( x + w, y + h, 0 );
			color_fn( col );
			vertex_fn( x, y + h, 0 );
			vertex_fn( x, y, 0 );

			gl_end( );
		}

		inline auto rect( float x, float y, float w, float h, engine::color_t col ) -> void
		{
			gl_start( gl_primitive_mode::line_strip );

			const static auto color_fn = reinterpret_cast< void( * )( engine::color_t ) > ( mono_lib::method( E( "UnityEngine::GL.Color()" ), 1 ) );
			if ( !engine::mem::valid_pointer( color_fn ) )
				return;

			const static auto vertex_fn = reinterpret_cast< void( * )( float, float, float ) >( mono_lib::method( E( "UnityEngine::GL.Vertex3()" ), -1 ) );
			if ( !engine::mem::valid_pointer( vertex_fn ) )
				return;

			color_fn( col );
			vertex_fn( x, y, 0 );
			vertex_fn( x + w, y, 0 );
			vertex_fn( x + w, y + h, 0 );
			vertex_fn( x, y + h, 0 );
			vertex_fn( x, y, 0 );

			gl_end( );
		}
	}
}