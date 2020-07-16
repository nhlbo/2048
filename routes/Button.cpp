#include "..\controls\Button.h"

Button::Button() {
	m_shape.setTexture(&m_texture);
}

sf::Vector2f Button::getPosition() {
	return m_shape.getPosition();
}

void Button::setTexture(const std::string& _texture) {
	m_texture.loadFromFile(_texture);
}

void Button::setTexture(const char* _texture) {
	m_texture.loadFromFile(_texture);
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
