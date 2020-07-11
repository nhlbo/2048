#include <iostream>
#include <SFML/Graphics.hpp>

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
	RenderWindow* window;
	RectangleShape backgroundTable, cellsColor[4][4], newGameButton, scoreBoard, bestScoreBoard;
	Text newGameTitle, scoreTitle, scoreBoardTitle, bestScoreTitle, bestScoreBoardTitle, loseTitle, cellsText[4][4];
	Font font;
	int table[4][4];
	int score, bestScore;

	void newGame();
	void update();
	void render();
	void renderText(Text& text, string str, Color color, int fontSize, int x, int y);
	string pointToString(int point);
	int getFontSize(int point);
	Color getCellColor(int val);
	void updateCells();
	bool isLose();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void saveBestScore();
	void loadBestScore();
	void saveTable();
	void loadTable();
};