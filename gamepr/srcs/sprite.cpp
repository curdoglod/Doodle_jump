#include "../includes/sprite.h"

//#include "ResourcePath.hpp" 

Sprite::Sprite(const std::string& texturePath, sf::RenderWindow* window) : m_window(window)
{
    //if (!m_texture.loadFromFile( resourcePath()+ texturePath)) pro MacOS
    if (!m_texture.loadFromFile( texturePath))
    {

    }


    m_sprite.setTexture(m_texture);
}

void Sprite::draw()
{
    m_window->draw(m_sprite);
}

void Sprite::draw(const float& x, const float& y) {
    m_sprite.setPosition(x, y);
    m_window->draw(m_sprite);
}

void Sprite::setPosition(float x, float y)
{
    m_sprite.setPosition(x, y);
}

void Sprite::setSize(float scaleX, float scaleY)
{
    m_sprite.setScale(scaleX, scaleY);
}

std::pair<float, float> Sprite::getSize() {

    return std::make_pair(m_texture.getSize().x, m_texture.getSize().y);
}

Sprite::~Sprite() {
}

