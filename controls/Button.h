#ifndef TITLE_H
#define TITLE_H
#include <SFML/Graphics.hpp>
#include <string>

#define TEXTURE_WIDTH m_texture.getSize().x
#define TEXTURE_HEIGHT m_texture.getSize().y

class Button {
	// <- this is private (by default)
	sf::Texture m_texture;
	sf::RectangleShape m_shape;

public:
	Button();
	sf::Vector2f getPosition();
	void setSize(float width, float height);
	void setTexture(const char* _texture);
	void setPosition(float x, float y);
	bool clicked(sf::RenderWindow* window);
	void draw(sf::RenderWindow* window);
};
#endif
