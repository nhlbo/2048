#ifndef _2048_CLASSIC_H
#define _2048_CLASSIC_H
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "Resourcepack.h"
#include "Button.h"
#include "Cell.h"
#include "Picture.h"

namespace Game { class Classic; }

class Game::Classic {
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

	int N;
	Cell cells[4][4];

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
	void draw(Cell cell[4][4], int remove_i = -1, int remove_j = -1);
	void display();

	// Main Game function:
	void newGame();
	void update();
	void render();
	void renderText(sf::Text& text, std::string str, sf::Color color, int fontSize, int x, int y);

	// Cells function: move, new, animation
	void moving_animation(int i, int j, int x, int y);
	void newcell_animation(int i, int j);
	void newCells();
	bool moveLeft();
	bool moveRight();
	bool moveUp();
	bool moveDown();

	bool isLose();
	void saveBestScore();
	void loadBestScore();
	void saveTable();
	void loadTable();
};
#endif

