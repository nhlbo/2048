#ifndef CELL_H
#define CELL_H
#include <SFML/Graphics.hpp>

#define TEXTURE_SIZE m_skin.getSize().y
#define WIDTH m_size.x
#define HEIGHT m_size.y

class Cell {
	// <- this is private (by default)
	// static attribute
	static sf::Vector2f m_size;
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
	sf::RectangleShape getShape();
	sf::Vector2f getPosition();
	int getVal();
	static void setSize(float width, float height);
	static void setTexture(const std::string _texture);
	static void setTexture(const char* _texture);
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
