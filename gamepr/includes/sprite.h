#include <SFML/Graphics.hpp>
#include <string>


#ifndef SPRITE_H
#define SPRITE_H
class Sprite
{
public:
	Sprite(const std::string& texturePath, sf::RenderWindow* window);
	void draw();
	void draw(const float& x, const float& y);
	void setPosition(float x, float y);
	void setSize(float scaleX, float scaleY);
	std::pair<float, float> getSize();

	virtual ~Sprite();
private:
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::RenderWindow* m_window;
};

#endif // SPRITE_H