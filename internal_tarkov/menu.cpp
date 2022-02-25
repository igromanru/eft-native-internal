#include "menu.hpp"
#include "script_manager.hpp"

auto load_script_callback( ) {
	auto script = menu::vars::parsed_scripts.at( menu::vars::selected_script );

	scripting::object.parse_file( script.c_str( ) );
}

auto clear_scripts_callback( ) {
	menu::vars::selected_script = 0;
	menu::vars::parsed_scripts.clear( );

	scripting::object.paint_callbacks.clear( );
	scripting::object.update_callbacks.clear( );

	scripting::object.parse_scripts( );
}

auto refresh_scripts_callback( ) {
	menu::vars::parsed_scripts.clear( );
	scripting::object.parse_scripts( );
}

auto menu::render( ) -> void
{
	menu::vars::main_menu_open = menu->window( E( "chloehack" ), { 50, 50 }, { 460, 350 }, false );

	if ( menu::vars::main_menu_open )
	{
		switch ( menu->tabs( { E( "rage" ), E( "visuals" ), E( "misc" ) } ) )
		{
		case 0:
		{
			menu->combobox( E( "aim" ), { 120, 45 }, { 160, 285 } );
			{
				menu->checkbox( E( "enabled" ), menu::vars::aimbot_enabled );

				menu->dropdown( E( "aim type" ), { E( "aimlock" ), E( "silent" ) }, menu::vars::aim_type, menu::vars::aim_type_opened );
				menu->slider( E( "aim fov" ), menu::vars::aimbot_fov, 0, 180, E( "px" ) );
				if ( menu::vars::aim_type == 0 )
				{
					menu->keybind( E( "aim key" ), menu::vars::aim_key );
					menu->dropdown( "aim mouse key", { E( "mouse 1" ), E( "mouse 2" ), E( "mouse 4" ), E( "mouse 5" ) }, menu::vars::mouse_aim_key, menu::vars::aim_key_opened );
				}
				menu->dropdown( E( "aimbone" ), { E( "head" ), E( "neck" ), E( "chest" ), E( "pelvis" ) }, menu::vars::aim_bone, menu::vars::aim_bone_opened );
			}

			menu->combobox( E( "weapon" ), { 290, 45 }, { 160, 115 } );
			{
				menu->checkbox( E( "no recoil" ), menu::vars::no_recoil );
				menu->checkbox( E( "no spread" ), menu::vars::no_spread );
				menu->checkbox( E( "no sway" ), menu::vars::no_sway );
				menu->checkbox( E( "instant ads" ), menu::vars::instant_ads );
			}

			break;
		}

		case 1: {
			switch ( menu->sub_tabs( { E( "player esp" ), E( "misc esp" ) } ) )
			{
			case 0: {
				menu->combobox( E( "players" ), { 120, 55 }, { 160, 275 } ); {

					menu->checkbox( E( "enabled" ), menu::vars::player_esp );
					menu->dropdown( E( "player type" ), { E( "pmc" ), E( "scav" ), E( "player scav" ) }, menu::vars::esp_type, menu::vars::esp_type_opened );

					switch ( menu::vars::esp_type )
					{
					case 0:
						menu->multi_dropdown( E( "options" ), { E( "box" ),E( "name" ), E( "health" ), E( "ammo" ), E( "weapon" ) }, menu::vars::player_esp_options, menu::vars::player_esp_options_opened );
						break;
					case 1:
						menu->multi_dropdown( E( "options" ), { E( "box" ), E( "name" ),E( "health" ),E( "ammo" ), E( "weapon" ) }, menu::vars::scav_esp_options, menu::vars::scav_esp_options_opened );
						break;
					case 2:
						menu->multi_dropdown( E( "options" ), { E( "box" ), E( "name" ), E( "health" ), E( "ammo" ), E( "weapon" ) }, menu::vars::player_scav_options, menu::vars::player_scav_options_opened );
						break;
					}
				}

				menu->combobox( E( "chams" ), { 290, 55 }, { 160, 120 } );
				{
					menu->checkbox( E( "enabled" ), menu::vars::chams );
					//menu->colorpicker( E( "color visible" ), menu::vars::color_visible, menu::vars::color_v_opened, false );
					//menu->colorpicker( E( "color behind" ), menu::vars::color_invisible, menu::vars::color_iv_opened, false );
				}

				menu->combobox( E( "flags" ), { 290, 184 }, { 160, 146 } );
				{
					menu->multi_dropdown( E( "pmc flags" ), { /*E( "kdr" ),*/ E( "distance" ), E( "level" ) }, menu::vars::player_esp_flags, menu::vars::player_esp_flags_opened );
					menu->multi_dropdown( E( "scav flags" ), { E( "distance" ) }, menu::vars::scav_esp_flags, menu::vars::scav_esp_flags_opened );
					menu->multi_dropdown( E( "player scav flags" ), { E( "distance" ) }, menu::vars::player_scav_flags, menu::vars::player_scav_flags_opened );
				}
				break;
			}

			case 1: {
				menu->combobox( E( "world" ), { 120, 55 }, { 160, 180 } );
				{
					menu->checkbox( E( "extract esp" ), menu::vars::extract_esp );
					if ( menu::vars::extract_esp )
					{
						menu->multi_dropdown( E( "flags" ), { E( "distance" ) }, menu::vars::extract_esp_flags, menu::vars::extract_esp_flags_opened );
					}
				}

				menu->combobox( E( "screen" ), { 290, 55 }, { 160, 210 } );
				{
					//menu->colorpicker( E( "" ), menu::vars::fov_color, menu::vars::fov_color_opened, true );

					menu->checkbox( E( "fov circle" ), menu::vars::fov_circle );
					menu->slider( E( "camera fov" ), menu::vars::camera_fov, 0, 50, E( "deg" ) );

					menu->checkbox( E( "aspect ratio changer" ), menu::vars::aspect_ratio_changer );
					if ( menu::vars::aspect_ratio_changer )
						menu->slider( E( "aspect ratio" ), menu::vars::aspect_ratio, 0, 100, E( "%" ) );

					menu->checkbox( E( "hitmarker" ), menu::vars::hitmarker );

					menu->checkbox( E( "indicators" ), menu::vars::widget );

					menu->multi_dropdown( E( "camera fx" ), { E( "no visor" ), E( "night vision" ), E( "thermal vision" ) }, menu::vars::camera_fx, menu::vars::camera_fx_opened );
				}
				break;
			}
			}

			break;
		}

		case 2:
		{
			menu->combobox( E( "misc" ), { 120, 45 }, { 160, 285 } );
			{
				menu->checkbox( E( "infinite stamina" ), menu::vars::inf_stamina );
				menu->checkbox( E( "walk on air" ), menu::vars::flyhack );

				menu->checkbox( E( "speedhack" ), menu::vars::speedhack );
				if ( menu::vars::speedhack )
				{
					menu->slider( E( "speedhack amount" ), menu::vars::speedhack_amnt, 0, 100, E( "%" ) );
					menu->keybind( E( "speedhack key" ), menu::vars::speedhack_key );
				}

				menu->checkbox( E( "max search stats" ), menu::vars::max_search_stat );
				menu->dropdown( E( "profile category" ), { E( "default" ), E( "eod" ), E( "developer" ), E( "sherpa" ), E( "emissary" ) }, menu::vars::profile_category, menu::vars::profile_cat_open );
			}

			menu->combobox( E( "scripts" ), { 290, 45 }, { 160, 285 } );
			{
				menu->listbox( "scripts_lb", menu::vars::parsed_scripts, menu::vars::selected_script );
				menu->button( E( "load script" ), load_script_callback );
				menu->button( E( "unload all scripts" ), clear_scripts_callback );
				menu->button( E( "refresh scripts" ), refresh_scripts_callback );
			}
			break;
		}
		}
	}
	menu->end_window( );

	if ( menu::vars::widget )
	{
		widget->window( E( "indicators" ), { 150, 450 }, { 100, 75 }, true ); {
			widget->text( menu::vars::ammo_count.c_str( ), { 5, 26 } );
			widget->text( menu::vars::fire_mode.c_str( ), { 5, 40 } );
		}

		widget->end_window( );
	}

	menu->watermark( );
}