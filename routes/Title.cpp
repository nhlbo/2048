#include "..\controls\Title.h"

sf::Vector2f Title::getPosition() {
	return m_shape.getPosition();
}

void Title::setPosition(float x, float y) {
	m_shape.setPosition(sf::Vector2f(x, y));
}

bool Title::click(sf::RenderWindow* window) {
	int coorMouse_x = sf::Mouse::getPosition(*window).x;
	int coorMouse_y = sf::Mouse::getPosition(*window).y;
	return (m_shape.getPosition().x <= coorMouse_x && coorMouse_x <= m_shape.getPosition().x + m_shape.getLocalBounds().width
		 && m_shape.getPosition().y <= coorMouse_y && coorMouse_y <= m_shape.getPosition().y + m_shape.getLocalBounds().height);
}
