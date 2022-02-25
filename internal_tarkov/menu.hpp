#pragma once
#include "framework.hpp"

namespace menu 
{
	namespace vars 
	{
		inline bool aimbot_enabled{ false };
		inline int aimbot_fov{ 180 };

		inline int aim_type;
		inline bool aim_type_opened;

		inline int aim_key;
		inline int mouse_aim_key;
		inline int flyhack_key;
		inline int speedhack_key;
		inline int camera_fx_key;

		inline bool aim_key_opened;

		inline int aim_bone;
		inline bool fov_circle;

		inline int camera_fov;

		inline bool aim_bone_opened;

		inline bool no_recoil{ false };
		inline bool no_sway{ false };
		inline bool no_spread{ false };
		
		inline bool flyhack{ false };
		inline bool max_search_stat{ false };
		inline bool inf_stamina{ false };

		inline bool aspect_ratio_changer{ false };
		inline int aspect_ratio;

		inline bool ricochet{ false };

		inline bool speedhack{ false };
		inline int speedhack_amnt{ false };

		inline bool instant_ads;

		inline std::vector<bool> camera_fx{ 0,0,0,0,0,0,0 };
		inline bool camera_fx_opened;

		inline int esp_type;
		inline bool esp_type_opened;

		inline bool player_esp;
		inline bool scav_esp;
		inline bool player_scav_esp;
		inline bool corpse_esp;

		inline bool loot_thru_walls;

		inline bool hitmarker;
		inline bool out_of_view_arrows;

		inline bool chams;

		inline std::vector<bool> player_esp_options{ 0,0,0,0,0,0,0 };
		inline std::vector<bool> player_esp_flags{ 0,0,0,0,0,0,0 };

		inline engine::color_t color_visible;
		inline engine::color_t color_invisible;
		inline bool color_iv_opened;
		inline bool color_v_opened;

		inline engine::color_t oof_arrow_color;
		inline bool oof_arrow_opened;

		inline engine::color_t fov_color{ 255, 0, 255, 255 };
		inline bool fov_color_opened;

		inline bool player_esp_options_opened;
		inline bool player_esp_flags_opened;

		inline std::vector<bool> scav_esp_options{ 0,0,0,0,0,0,0 };
		inline std::vector<bool> scav_esp_flags{ 0,0,0,0,0,0,0 };

		inline bool scav_esp_options_opened;
		inline bool scav_esp_flags_opened;

		inline std::vector<bool> player_scav_options{ 0,0,0,0,0,0,0 };
		inline std::vector<bool> player_scav_flags{ 0,0,0,0,0,0,0 };
		
		inline bool player_scav_options_opened;
		inline bool player_scav_flags_opened;

		inline bool item_esp;
		inline int item_esp_max_distance;

		inline std::vector<bool> item_esp_flags{ 0,0,0,0,0,0,0 };
		inline bool item_esp_flags_opened;

		inline bool extract_esp;
		inline std::vector<bool> extract_esp_flags{ 0,0,0,0,0,0,0 };
		inline bool extract_esp_flags_opened;

		inline bool widget{ false };
		inline bool main_menu_open{ false };

		inline std::wstring ammo_count;
		inline std::wstring fire_mode;

		inline int profile_category;
		inline bool profile_cat_open;

		inline std::vector< std::string > parsed_scripts;
		inline int selected_script;
		inline bool load_script_clicked;
		inline bool unload_script_clicked;
	}

	auto render( ) -> void;
	inline std::shared_ptr<c_menu_framework> menu = std::make_shared<c_menu_framework>( );
	inline std::shared_ptr<c_menu_framework> widget = std::make_shared<c_menu_framework>( );
}