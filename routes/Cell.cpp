#include "..\controls\Cell.h"

// identify static attributes (default)
sf::Vector2f Cell::m_size(sf::Vector2f(100, 100));
sf::Texture Cell::m_skin(creatTexture("data/texture/4x4-1.png"));
sf::Font Cell::m_font(creatFont("data/ClearSans-Bold.ttf"));

sf::Texture Cell::creatTexture(const char* _texture) {
	sf::Texture texture;
	texture.loadFromFile(_texture);
	return texture;
}

sf::Font Cell::creatFont(const char* _font) {
	sf::Font font;
	font.loadFromFile(_font);
	return font;
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
	// Update Text
	m_text.setString((m_data == 0 ? "" : std::to_string(m_data)));
	m_text.setCharacterSize(getTextSize(m_data));
	m_text.setFillColor((m_data <= 4 ? sf::Color(108, 99, 91) : sf::Color::White));
	m_text.setPosition(sf::Vector2f(m_pos_text.x - m_text.getLocalBounds().width / 2, m_pos_text.y - m_text.getLocalBounds().height));
	// Update Texture
	if (m_data == 0) {
		m_shape.setTextureRect(sf::IntRect(0, 0, WIDTH, HEIGHT));
	}
	else {
		m_shape.setTextureRect(sf::IntRect(0 + 128 * log2(m_data), 0, WIDTH, HEIGHT));
	}
}

Cell::Cell() {
	m_shape.setSize(m_size);
	m_shape.setTexture(&m_skin);
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

void Cell::setSize(float width, float height) {
	Cell::m_size = sf::Vector2f(width, height);
}

void Cell::setTexture(const char* _texture) {
	Cell::m_skin.loadFromFile(_texture);
}

void Cell::setFont(const char* _font) {
	Cell::m_font.loadFromFile(_font);
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
