#pragma once
#include "engine_render.hpp"
#include "geometry.hpp"

#include "draw_list.hpp"

class c_menu_framework {
public:
	bool window( const std::string& title, const geometry::vec2_t& window_pos, const geometry::vec2_t& window_size, bool always_open );

	void watermark( );

	void end_window( );
	short tabs( const std::vector<std::string> names );

	short sub_tabs( const std::vector<std::string> names );

	void button( const std::string& title, void* callback );

	void text( const std::wstring& text, const geometry::vec2_t& pos );
	void combobox( const std::string& title, const geometry::vec2_t& pos, const geometry::vec2_t& size );
	void keybind( const std::string& title, int& value );
	void checkbox( const std::string& title, bool& value );
	void slider( const std::string& title, int& value, int min, int max, const std::string& suffix );
	void dropdown( const std::string& title, const std::vector<std::string> items, int& value, bool& is_open );
	void listbox( const std::string& title, const std::vector<std::string> items, int& selected_index );

	void multi_dropdown( const std::string& title, const std::vector<std::string> items, std::vector<bool>& value, bool& is_open );
	void colorpicker( const std::string& title, engine::color_t& value, bool& is_open, bool inlined );
	bool is_menu_open{ true };

private:
	geometry::vec2_t window_pos{ 50, 50 }, mouse_pos, next_item_pos;
	geometry::vec2_t window_size;
	geometry::vec2_t old_mouse_pos;
	bool mouse_state, insert_state;
	engine::event_type menu_event;
	bool old_mouse_state, old_insert_state;

	geometry::vec2_t widget_mouse_pos, old_widget_mouse_pos;
	bool widget_state, old_widget_state;


	bool interacting;
	bool is_mouse_in_box( const geometry::vec2_t& pos1, const geometry::vec2_t& pos2 );
	float menu_alpha;
	float color_picker_alpha;

	draw_list_t* overlay_drawlist;
};