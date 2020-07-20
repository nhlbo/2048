#ifndef ANIMATION_H
#define ANIMATION_H
#include <SFML/Graphics.hpp>
#include "Picture.h"

#define STEP 5
#define APPEAR 50	// 50px of size

class Animation {
	class Member {
		sf::RectangleShape m_shape;
		sf::Vector2f m_dis;

	public:
		Member(sf::RectangleShape u, sf::Vector2f dis);
		sf::RectangleShape getShape();
		void addSize(float d);
		void addPosition(float d);
		void move();
	};

	std::vector<Member> m_data;

public:
	void push(sf::RectangleShape u, sf::Vector2f dis);
	void clear();

	// Animation function
	void appear(sf::RenderWindow* window, Picture& frame);
	void move(sf::RenderWindow* window, Picture& frame);
};

#endif // !ANIMATION_H
