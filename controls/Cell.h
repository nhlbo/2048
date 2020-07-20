#ifndef CELL_H
#define CELL_H
#include <SFML/Graphics.hpp>

#define TEXTURE_SIZE m_skin.getSize().y

class Cell {
	// <- this is private (by default)
	// static attribute
	static sf::Texture m_skin;

	// member attribute
	int m_data;
	sf::RectangleShape m_shape;

private:
	// inside function
	static sf::Texture creatTexture(const char* _texture);
	static int getTextSize(int point);
	void update(int val = -1);

public:
	Cell();

	// member function
	static void setTexture(const std::string _texture);
	static void setTexture(const char* _texture);
	sf::Vector2f distance(Cell& v);
	sf::RectangleShape getShape();
	sf::Vector2f getPosition();
	int getVal();
	void setSize(float width, float height);
	void setPosition(float x, float y);
	void setPosition(sf::Vector2f pos);
	void setOutlineThickness(float thickness);
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
