#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "Cell.h"

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
	//const int coor[4] = { 20, 180, 340, 500 };
	//const int coorText[4] = { 70, 220, 280, 540 };

	const int size[3][3] = { 20, 20, 150, 22, 22, 96, 20, 20, 70 };	// size[i] = { first_pos.x, first_pos.y, size_of_cell }	(4x4, 6x6, 8x8)
	int m_size_i;
	int m_size;		// MAX 8
	Cell cells[8][8];

	RenderWindow* window;
	Texture skin, skin_b;
	Sprite background;
	Sprite board;

	RectangleShape backgroundTable, newGameButton, scoreBoard, bestScoreBoard;
	Text newGameTitle, scoreTitle, scoreBoardTitle, bestScoreTitle, bestScoreBoardTitle, loseTitle;
	Font font;
	
	int score, bestScore;
	bool firstLoad;

	void newGame();
	void update();
	void render();
	void renderText(Text& text, string str, Color color, int fontSize, int x, int y);

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