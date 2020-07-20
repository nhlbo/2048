#include "..\controls\Animation.h"

Animation::Member::Member(sf::RectangleShape u, sf::Vector2f dis) {
	m_shape = u;
	m_dis = dis;
}

sf::RectangleShape Animation::Member::getShape() {
	return m_shape;
}

void Animation::Member::addSize(float d) {
	sf::Vector2f size = m_shape.getSize();
	m_shape.setSize(sf::Vector2f(size.x + d, size.y + d));
}

void Animation::Member::addPosition(float d) {
	sf::Vector2f pos = m_shape.getPosition();
	m_shape.setPosition(sf::Vector2f(pos.x + d, pos.y + d));
}

void Animation::Member::move() {
	sf::Vector2f pos = m_shape.getPosition();
	m_shape.setPosition(sf::Vector2f(pos.x + m_dis.x / STEP, pos.y + m_dis.y / STEP));
}

void Animation::push(sf::RectangleShape u, sf::Vector2f dis) {
	m_data.push_back(Member(u, dis));
}

void Animation::clear() {
	m_data.clear();
}

void Animation::appear(sf::RenderWindow* window, Picture& frame) {
	if (m_data.size() == 0) return;
	sf::Time delay = sf::milliseconds(18);

	// optimize animation
	sf::Texture tex;
	tex.create(950, 720);
	tex.update(*window);
	sf::Sprite background(tex);

	// for auto of vector (C++11)
	// set all cell to small
	for (Member& u : m_data) {
		u.addPosition(APPEAR / 2);
		u.addSize(-APPEAR);
	}
		
	for (int i = 0; i < STEP; ++i) {
		sleep(delay);
		window->clear();
		window->draw(background);
		for (Member& u : m_data) {
			u.addPosition(-(APPEAR / 2 / STEP));
			u.addSize(APPEAR / STEP);
			window->draw(u.getShape());
		}
		frame.draw(window);
		window->display();
	}
	clear();
}

void Animation::move(sf::RenderWindow* window, Picture& frame) {
	if (m_data.size() == 0) return;
	sf::Time delay = sf::milliseconds(15);

	// optimize animation
	sf::Texture tex;
	tex.create(950, 720);
	tex.update(*window);
	sf::Sprite background(tex);

	// for auto of vector (C++11)
	for (int i = 0; i < STEP; ++i) {
		sleep(delay);
		window->clear();
		window->draw(background);
		for (Member& u : m_data) {
			u.move();
			window->draw(u.getShape());
		}
		frame.draw(window);
		window->display();
	}
	clear();
}
