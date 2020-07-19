#include "..\controls\Picture.h"

Picture::Picture() {
	m_shape.setTexture(m_texture);
}

sf::Vector2f Picture::getPosition() {
	return m_shape.getPosition();
}

void Picture::setTexture(const std::string& _texture) {
	m_texture.loadFromFile(_texture);
	m_shape.setTextureRect(sf::IntRect(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT));
}

void Picture::setTexture(const char* _texture) {
	m_texture.loadFromFile(_texture);
	m_shape.setTextureRect(sf::IntRect(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT));
}

void Picture::draw(sf::RenderWindow* window) {
	window->draw(m_shape);
}
