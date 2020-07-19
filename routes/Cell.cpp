#include "..\controls\Cell.h"

// identify static attributes (default)
sf::Vector2f Cell::m_size(sf::Vector2f(100, 100));
sf::Texture Cell::m_skin(creatTexture("resourcepacks/classical/textures/block.png"));

sf::Texture Cell::creatTexture(const char* _texture) {
	sf::Texture texture;
	texture.loadFromFile(_texture);
	return texture;
}

int Cell::getTextSize(int point) {
	int cnt = 0;
	while (point > 0) {
		point /= 10;
		cnt++;
	}
	if (cnt == 1) return 40;
	if (cnt == 2) return 40;
	if (cnt == 3) return 35;
	if (cnt == 4) return 30;
	if (cnt == 5) return 25;
	if (cnt == 6) return 20;
	return 0;
}

void Cell::update(int val) {
	if (val > -1) m_data = val;
	m_shape.setSize(m_size);
	if (m_data == 0) {
		m_shape.setTexture(&m_skin);
		m_shape.setTextureRect(sf::IntRect(0, 0, TEXTURE_SIZE, TEXTURE_SIZE));
	}
	else {
		m_shape.setTextureRect(sf::IntRect(0 + TEXTURE_SIZE * log2(m_data), 0, TEXTURE_SIZE, TEXTURE_SIZE));
	}
}

Cell::Cell() {
	update(0);
}

sf::RectangleShape Cell::getShape() {
	return m_shape;
}

sf::Vector2f Cell::getPosition() {
	return m_shape.getPosition();
}

int Cell::getVal() {
	return m_data;
}

void Cell::setSize(float width, float height) {
	Cell::m_size = sf::Vector2f(width, height);
}

void Cell::setTexture(const std::string _texture) {
	Cell::m_skin.loadFromFile(_texture);
}

void Cell::setTexture(const char* _texture) {
	Cell::m_skin.loadFromFile(_texture);
}

void Cell::setPosition(float x, float y) {
	m_shape.setPosition(sf::Vector2f(x, y));
}

void Cell::setPosition(sf::Vector2f pos) {
	m_shape.setPosition(pos);
}

void Cell::setOutlineThickness(float thickness) {
	m_shape.setOutlineThickness(thickness);
}

void Cell::draw(sf::RenderWindow* window) {
	if (m_data == 0) return;
	window->draw(m_shape);
}


bool Cell::operator==(int val) { return m_data == val; }

bool Cell::operator==(const Cell& v) { return m_data == v.m_data; }

bool Cell::operator!=(int val) { return m_data != val; }

bool Cell::operator!=(const Cell& v) { return m_data != v.m_data; }

void Cell::operator=(int val) { update(val); }

void Cell::operator=(const Cell& v) { update(v.m_data); }

void Cell::operator+=(int val) { update(m_data + val); }

void Cell::operator+=(const Cell& v) { update(m_data + v.m_data); }

