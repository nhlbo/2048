#ifndef PICTURE_H
#define PICTURE_H
#include <SFML/Graphics.hpp>
#include <string>

#define TEXTURE_WIDTH m_texture.getSize().x
#define TEXTURE_HEIGHT m_texture.getSize().y

class Picture {
	// <- this is private (by default)
	sf::Texture m_texture;
	sf::Sprite m_shape;

public:
	Picture();
	sf::Vector2f getPosition();
	void setTexture(const std::string& _texture);
	void setTexture(const char* _texture);
	template<class T> void setPosition(std::pair<T, T> pos) {
		m_shape.setPosition(sf::Vector2f(pos.first, pos.second));
	}

	void draw(sf::RenderWindow* window);
};
#endif

