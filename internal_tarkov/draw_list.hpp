#pragma once
#include "engine_render.hpp"

enum draw_type : int {
    rect = 1,
    box,
    text,
    gradient,
    line
};

class draw_element_t {
public:
    engine::rect_t pos;
    managed_system::c_string text = E( L"" );
    bool centered = false;
    bool outlined = false;
    bool vertical = false;
    engine::color_t clr1 = { 0,0,0,0 };
    engine::color_t clr2 = { 0,0,0,0 };
    draw_type type;

    draw_element_t( void ) {};
};

class draw_list_t {
public:
    draw_list_t( ) : elements( {} ) {};

    void text( engine::rect_t pos, managed_system::c_string text, engine::color_t clr, bool outlined = false, bool centered = false );
    void rect( engine::rect_t pos, engine::color_t clr );
    void rect( geometry::vec2_t pos1, geometry::vec2_t pos2, engine::color_t clr );
    void box( engine::rect_t pos, engine::color_t clr );
    void box( geometry::vec2_t pos1, geometry::vec2_t pos2, engine::color_t clr );
    void line( geometry::vec2_t start, geometry::vec2_t end, engine::color_t clr );
    void gradient( engine::rect_t pos, engine::color_t clr1, engine::color_t clr2, bool vertical );
    void gradient( geometry::vec2_t pos1, geometry::vec2_t pos2, engine::color_t clr1, engine::color_t clr2, bool vertical );
    void render( );
private:
    std::vector<draw_element_t> elements;
};