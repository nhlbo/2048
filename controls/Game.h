#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "Cell.h"

using namespace sf;
using namespace std;

class Game {
public:
	Game();
	~Game();
	void start();

private:
	const int coor[4] = { 20, 130, 240, 350 };
	const int coorText[4] = { 70, 180, 290, 400 };
	Cell cells[4][4];

	RenderWindow* window;
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