#ifndef _2048_BRICK_H
#define _2048_BRICK_H
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "Resourcepack.h"
#include "Button.h"
#include "Cell.h"
#include "Picture.h"

namespace Game { class Brick; }

class Game::Brick {
public:
	void init(sf::RenderWindow* __window, Resourcepack* __res, sf::Music* __music);
	bool loadResourcepack();
	void start();

private:
	// shared attribute 
	sf::RenderWindow* window;
	Resourcepack* res;
	sf::Music* music;

	sf::Font font;
	Picture background, frame;
	sf::RectangleShape loseBackground;
	Button newGameButton, scoreBoard, bestScoreBoard, tryAgainButton, backToMenu;
	sf::Text scoreTitle, bestScoreTitle, loseTitle;
	std::pair<int,int> cur;
	Cell cells[7][5], next;

	int score, bestScore;
	bool firstLoad;
	bool isMainMenu;
	bool isGameOver;

	// RenderWindow function: clear, draw, display
	void clear(sf::Color color = sf::Color(0, 0, 0, 255));
	void draw(sf::RectangleShape& shape);
	void draw(sf::Sprite& shape);
	void draw(Picture& picture);
	void draw(Button& button);
	void draw(sf::Text& text);
	void draw(Cell& cell);
	void draw(Cell cell[7][5], int remove_i = -1, int remove_j = -1);
	void display();

	// Main Game function:
	void newGame();
	void update();
	void render();
	void renderText(sf::Text& text, std::string str, sf::Color color, int fontSize, int x, int y);

	// Cells function: move, new, animation
	void moving_animation(int i, int j, int u, int v);
	void newcell_animation(int u = 0, int v = 2);
	void newCells();
	bool moveCells(int move);		// 0 - left, 1 - right, 2 - down	(cur cell)
	void merge(int u, int v);	// merge cell use recursion

	bool isLose();
	void saveBestScore();
	void loadBestScore();
	void saveTable();
	void loadTable();
};


#endif

