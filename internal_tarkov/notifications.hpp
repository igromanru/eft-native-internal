#pragma once
#include <vector>
#include <memory>
#include "engine_render.hpp"

namespace features
{
	class c_notify_text
	{
	public:
		managed_system::c_string m_text;
		engine::color_t m_color;
		float m_time;
		c_notify_text( const managed_system::c_string text, engine::color_t color, float time ) : m_text( text ), m_color( color ), m_time( time ) { }
	};

	class c_notify
	{
	private:
		std::vector< std::shared_ptr< features::c_notify_text > > m_notify_text;
	public:
		c_notify( ) : m_notify_text{} {}

		auto push( managed_system::c_string text, float timestamp, engine::color_t color = engine::color_t( 255, 255, 255, 180  ) ) -> void {
			m_notify_text.push_back( std::make_shared< features::c_notify_text >( text, color, timestamp ) );
		}

		auto run( ) -> void
		{
			MONO_METHOD( get_time_fn, E( "UnityEngine::Time.get_time()" ), 0, float( * )( ) );
			MONO_METHOD( abs_fn, "UnityEngine::Mathf.Abs()", 1, float( * )( float ) );

			for ( size_t idx{ 0 }; idx < m_notify_text.size( ) && !m_notify_text.empty( ); idx++ )
			{
				const auto& notify = m_notify_text[ idx ];

				if ( abs_fn( get_time_fn( ) - notify->m_time ) > 5.f )
				{
					m_notify_text.erase( m_notify_text.begin( ) + idx );

					if ( !m_notify_text.size( ) )
						return;
				}
			}

			if ( m_notify_text.empty( ) )
				return;

			for ( size_t idx{ 0 }; idx < m_notify_text.size( ); ++idx )
			{
				const auto& notify = m_notify_text[ idx ];

				float time_delta = abs_fn( get_time_fn( ) - notify->m_time );

				engine::color_t color = notify->m_color;

				float alpha = 180.f;
				float height = 16.f + ( 16.f * idx );

				if ( time_delta > 4.5f )
					alpha = ( 180.f - ( ( ( time_delta - ( 5.f - 0.2f ) ) / 0.2f ) * 255.f ) );

				if ( time_delta < 0.3f )
					alpha = ( ( time_delta / 0.3f ) * 180.f );

				float width = 3;

				if ( time_delta < 0.3f )
					width = ( time_delta / 0.3f ) * 3.f;

				color = { color.r * 255.f, color.g * 255.f, color.b * 255.f, alpha };

				engine::gui::string_with_background( width, height, 300, 20, notify->m_text, color, true );
			}
		}
	};

	inline c_notify notifcations;
}