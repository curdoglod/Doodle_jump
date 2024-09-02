#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics.hpp>
#include "sprite.h"
#include "engine.h"

class Object {
public:
    virtual void drawObj(const float& x, const float& y);
    virtual void drawObj();
    virtual bool crossing(Object* obj, const float& x_range, const float& y_range);
    virtual bool crossing(Object* obj);
    virtual std::pair<float, float> get_position();
    virtual void Set_position(const float& pos_x, const float& pos_y);
    virtual void Set_position_onPlatform(const float& pos_x, const float& pos_y);
    virtual void move_y(const float& pos_y);
    virtual void move_x(const float& pos_x);
    virtual std::pair<int, int>  get_size();
    virtual void init_size();
    virtual ~Object();
protected:
    Sprite* sprite;
    Engine* game;
    std::pair<float, float> position;
    std::pair<int, int> size;
};

#endif // OBJECT_H
