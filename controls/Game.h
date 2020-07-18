#ifndef GAME_H
#define GAME_H
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "Resourcepack.h"
#include "Cell.h"
#include "Button.h"

#define BOARD_SIZE 650
#define LINE_SIZE 10

using namespace sf;
using namespace std;

class Game {
public:
	Game();
	~Game();
	void start();

private:
	//const int coor[4] = { 20, 130, 240, 350 };
	//const int coorText[4] = { 70, 180, 290, 400 };

	const int size[3][3] = { 20, 20, 150, 22, 22, 96, 20, 20, 70 };	// size[i] = { first_pos.x, first_pos.y, size_of_cell }	(4x4, 6x6, 8x8)
	int m_size_i;
	int m_size;		// MAX 8

	RenderWindow* window;
	Music music_background;
	Resourcepack res;
	Font font;

	Texture skin_0, skin, skin_b;
	Sprite mainmenu, background, frame;
	RectangleShape loseBackground;
	Button newGameButton, scoreBoard, bestScoreBoard, tryAgainButton, backToMenu;
	Text scoreTitle, bestScoreTitle, loseTitle;
	Cell cells[8][8];
	
	int score, bestScore;
	bool firstLoad;
	bool isMainMenu;
	bool isGameOver;

	// Resourcepacks function
	bool loadResourcepack(const char* pack_name = "classical");

	// RenderWindow function: clear, draw, display
	void clear(Color color = Color(0, 0, 0, 255));
	void draw(RectangleShape& shape);
	void draw(Sprite& shape);
	void draw(Button& button);
	void draw(Text& text);
	void draw(Cell& cell);
	void display();

	// Main Game function:
	void newGame();
	void update();
	void render();
	void renderText(Text& text, string str, Color color, int fontSize, int x, int y);

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