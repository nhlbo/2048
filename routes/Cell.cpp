#include "..\controls\Cell.h"

// identify static attributes
sf::Vector2f Cell::m_size(sf::Vector2f(WIDTH, HEIGHT));
sf::Font Cell::m_font(creatFont(FONT));

sf::Font Cell::creatFont(const char* _font) {
	sf::Font font;
	font.loadFromFile(_font);
	return font;
}

sf::Color Cell::getColor(int val) {
	if (val == 0) return sf::Color(198, 184, 171);
	if (val == 2) return sf::Color(236, 224, 214);
	if (val == 4) return sf::Color(235, 220, 195);
	if (val == 8) return sf::Color(243, 168, 115);
	if (val == 16) return sf::Color(248, 139, 94);
	if (val == 32) return sf::Color(250, 113, 90);
	if (val == 64) return sf::Color(251, 85, 60);
	if (val == 128) return sf::Color(236, 201, 111);
	if (val == 256) return sf::Color(236, 198, 97);
	if (val == 512) return sf::Color(237, 194, 83);
	if (val == 1024) return sf::Color(237, 191, 71);
	if (val == 2048) return sf::Color(239, 187, 57);
	if (val == 4096) return sf::Color(240, 92, 97);
	if (val == 8192) return sf::Color(246, 67, 84);
	if (val == 16384) return sf::Color(249, 57, 62);
	if (val == 32768) return sf::Color(99, 168, 203);
	if (val == 65536) return sf::Color(73, 147, 212);
	if (val == 131072) return sf::Color(0, 116, 192);
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
	m_shape.setFillColor(getColor(m_data));
	m_text.setString((m_data == 0 ? "" : std::to_string(m_data)));
	m_text.setCharacterSize(getTextSize(m_data));
	m_text.setFillColor((m_data <= 4 ? sf::Color(108, 99, 91) : sf::Color::White));
	m_text.setPosition(sf::Vector2f(m_pos_text.x - m_text.getLocalBounds().width / 2, m_pos_text.y - m_text.getLocalBounds().height));
}

Cell::Cell() {
	m_shape.setSize(m_size);
	m_text.setFont(m_font);
	update(0);
}

sf::RectangleShape Cell::getShape() {
	return m_shape;
}

sf::Text Cell::getText() {
	return m_text;
}

int Cell::getVal() {
	return m_data;
}

void Cell::setPosition(float x, float y) {
	m_shape.setPosition(sf::Vector2f(x, y));
}

void Cell::setPositionText(float x, float y) {
	m_pos_text = sf::Vector2f(x, y);
}

void Cell::draw(sf::RenderWindow* window) {
	window->draw(m_shape);
	window->draw(m_text);
}


bool Cell::operator==(int val) { return m_data == val; }

bool Cell::operator==(const Cell& v) { return m_data == v.m_data; }

bool Cell::operator!=(int val) { return m_data != val; }

bool Cell::operator!=(const Cell& v) { return m_data != v.m_data; }

void Cell::operator=(int val) { update(val); }

void Cell::operator=(const Cell& v) { update(v.m_data); }

void Cell::operator+=(int val) { update(m_data + val); }

void Cell::operator+=(const Cell& v) { update(m_data + v.m_data); }
