#include "..\controls\Button.h"

Button::Button() {
	m_shape.setTexture(&m_texture);
}

sf::Vector2f Button::getPosition() {
	return m_shape.getPosition();
}

void Button::setSize(float width, float height) {
	m_shape.setSize(sf::Vector2f(width, height));
	m_shape.setTextureRect(sf::IntRect(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT));
}

void Button::setTexture(const char* _texture) {
	m_texture.loadFromFile(_texture);
}

void Button::setPosition(float x, float y) {
	m_shape.setPosition(sf::Vector2f(x, y));
}

bool Button::clicked(sf::RenderWindow* window) {
	int coorMouse_x = sf::Mouse::getPosition(*window).x;
	int coorMouse_y = sf::Mouse::getPosition(*window).y;
	return (m_shape.getPosition().x <= coorMouse_x && coorMouse_x <= m_shape.getPosition().x + m_shape.getLocalBounds().width
		 && m_shape.getPosition().y <= coorMouse_y && coorMouse_y <= m_shape.getPosition().y + m_shape.getLocalBounds().height);
}

void Button::draw(sf::RenderWindow* window) {
	window->draw(m_shape);
}
