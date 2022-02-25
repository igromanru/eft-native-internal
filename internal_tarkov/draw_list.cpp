#include "draw_list.hpp"

void draw_list_t::text( engine::rect_t pos, managed_system::c_string text, engine::color_t clr, bool outlined, bool centered ) 
{
    draw_element_t element;

    element.pos = pos;
    element.text = text;
    element.clr1 = clr;
    element.outlined = outlined;
    element.centered = centered;
    element.type = draw_type::text;

    this->elements.push_back( element );
}

void draw_list_t::rect( engine::rect_t pos, engine::color_t clr ) {
    draw_element_t element;

    element.pos = pos;
    element.clr1 = clr;
    element.type = draw_type::rect;

    this->elements.push_back( element );
}

void draw_list_t::rect( geometry::vec2_t pos, geometry::vec2_t size, engine::color_t clr ) {
    this->rect( { pos.x, pos.y, size.x, size.y }, clr );
}

void draw_list_t::box( engine::rect_t pos, engine::color_t clr ) {
    draw_element_t element;

    element.pos = pos;
    element.clr1 = clr;
    element.type = draw_type::box;

    this->elements.push_back( element );
}

void draw_list_t::box( geometry::vec2_t pos, geometry::vec2_t size, engine::color_t clr )
{
    this->box( { pos.x, pos.y, size.x, size.y }, clr );
}

void draw_list_t::line( geometry::vec2_t start, geometry::vec2_t end, engine::color_t clr ) {
    draw_element_t element;

    element.pos = { start.x, start.y, end.x, end.y };
    element.clr1 = clr;
    element.type = draw_type::line;

    this->elements.push_back( element );
}

void draw_list_t::gradient( engine::rect_t pos, engine::color_t clr1, engine::color_t clr2, bool vertical ) {
    draw_element_t element;

    element.pos = pos;
    element.clr1 = clr1;
    element.clr2 = clr2;
    element.vertical = vertical;
    element.type = draw_type::gradient;

    this->elements.push_back( element );
}

void draw_list_t::gradient( geometry::vec2_t pos1, geometry::vec2_t size, engine::color_t clr1, engine::color_t clr2, bool vertical ) {
    this->gradient( { pos1.x, pos1.y, size.x, size.y }, clr1, clr2, vertical );
}

void draw_list_t::render( )
{
    if ( engine::c_events::get_current_event( )->type( ) != engine::event_type::repaint )
        return;

    for ( draw_element_t element : elements ) 
    {
        switch ( element.type ) {
        case draw_type::rect:
            engine::gui::filled_rect( element.pos, element.clr1 );
            break;
        case draw_type::box:
            engine::gui::rect( element.pos.x, element.pos.y, element.pos.w, element.pos.h, element.clr1 );
            break;
        case draw_type::text:
            engine::gui::string( element.pos.x, element.pos.y, element.pos.w, element.pos.h, element.text, element.clr1, element.centered, element.outlined );
            break;
        case draw_type::gradient:
            if ( element.vertical )
                engine::gl::gradient_filled_rect( element.pos.x, element.pos.y, element.pos.w, element.pos.h, element.clr1, element.clr2 );
            else
                engine::gl::gradient_horizontal_filled_rect( element.pos.x, element.pos.y, element.pos.w, element.pos.h, element.clr1, element.clr2 );
            break;
        case draw_type::line:
            engine::gl::line( element.pos.x, element.pos.y, element.pos.w, element.pos.h, element.clr1 );
            break;
        default:
            break;
        }

        elements.clear( );
    }
}