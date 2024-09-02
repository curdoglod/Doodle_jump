#include "../includes/object.h"

void Object::drawObj(const float& x, const float& y) {
    Set_position(x, y);
    sprite->draw(x, y);
}

void Object::drawObj() {
    sprite->draw(position.first, position.second);
}

bool Object::crossing(Object* obj, const float& x_range, const float& y_range) {
    std::pair<float, float> temp_pos = obj->get_position();
    std::pair<float, float> temp_size = obj->get_size();

    return ((temp_pos.second + temp_size.second * y_range) >= (position.second)) &&
        (temp_pos.second) <= (position.second + size.second * y_range) &&
        (temp_pos.first + temp_size.first * x_range >= position.first) && temp_pos.first <= position.first + size.first * x_range;
}

bool Object::crossing(Object* obj) {
    return crossing(obj, 1.0f, 1.0f);
}

std::pair<float, float> Object::get_position() {
    return position;
}

void Object::Set_position(const float& pos_x, const float& pos_y) {
    position = std::make_pair(pos_x, pos_y);
}

void Object::Set_position_onPlatform(const float& pos_x, const float& pos_y) {
    init_size();
    Set_position(pos_x, pos_y - size.second);
}

void Object::move_y(const float& pos_y) {
    position.second += pos_y;
}

void Object::move_x(const float& pos_x) {
    position.first += pos_x;
}

std::pair<int, int> Object::get_size() {
    return size;
}

void Object::init_size() {
    size = sprite->getSize();
}

Object::~Object() {
    delete sprite;
}
