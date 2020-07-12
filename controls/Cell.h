#ifndef CELL_H
#define CELL_H
#include <SFML/Graphics.hpp>

#define WIDTH m_size.x
#define HEIGHT m_size.y

class Cell {
	// <- this is private (by default)
	// static attribute
	static sf::Vector2f m_size;
	static sf::Texture m_skin;
	static sf::Font m_font;

	// member attribute
	int m_data;
	sf::RectangleShape m_shape;
	sf::Text m_text;

	// position attribute
	sf::Vector2f m_pos_text;

private:
	// inside function
	static sf::Texture creatTexture(const char* _texture);
	static sf::Font creatFont(const char* _font);
	static int getTextSize(int point);
	void update(int val = -1);

public:
	Cell();

	// member function
	sf::RectangleShape getShape();
	sf::Text getText();
	int getVal();
	static void setSize(float width, float height);
	static void setTexture(const char* _texture);
	static void setFont(const char* _font);
	void setPosition(float x, float y);
	void setPositionText(float x, float y);
	void draw(sf::RenderWindow* window);

	// operator function (compare and change: m_data)
	bool operator==(int val);
	bool operator==(const Cell& v);		
	bool operator!=(int val);
	bool operator!=(const Cell& v);		
	void operator=(int val);			
	void operator=(const Cell& v);		
	void operator+=(int val);			
	void operator+=(const Cell& v);		
};
#endif
