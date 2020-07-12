#ifndef CELL_H
#define CELL_H
#include <SFML/Graphics.hpp>

#define FONT "data/ClearSans-Bold.ttf"
#define WIDTH 100
#define HEIGHT 100

class Cell {
	// <- this is private (by default)
	// static attribute
	static sf::Font m_font;
	static sf::Vector2f m_size;

	// member attribute
	int m_data;
	sf::RectangleShape m_shape;
	sf::Text m_text;

	// position attribute
	sf::Vector2f m_pos_text;

private:
	// inside function
	static sf::Font creatFont(const char* _font);
	static sf::Color getColor(int val);
	static int getTextSize(int point);
	void update(int val = -1);

public:
	Cell();

	// member function
	sf::RectangleShape getShape();
	sf::Text getText();
	int getVal();
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
