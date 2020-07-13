#ifndef TITLE_H
#define TITLE_H
#include <SFML/Graphics.hpp>

class Title {
	// <- this is private (by default)
	sf::Texture m_texture;
	sf::Sprite m_shape;

public:
	sf::Vector2f getPosition();
	void setPosition(float x, float y);
	bool click(sf::RenderWindow* window);
};
#endif
