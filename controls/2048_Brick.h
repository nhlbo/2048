#ifndef _2048_BRICK_H
#define _2048_BRICK_H
#include "Animation.h"
#include "Resourcepack.h"
#include "Button.h"
#include "Cell.h"
#include "Picture.h"

using namespace sf;
using namespace std;

namespace G2048 { class Brick; }

class G2048::Brick {
public:
	void init(RenderWindow* __window, Resourcepack* __res, Music* __music);
	bool loadResourcepack();
	void start();

private:
	// shared attribute 
	RenderWindow* window;
	Resourcepack* res;
	Music* music;

	// Animation attribute
	Animation animation;
	RectangleShape copy[7][5];		// old-version of cells

	Font font;
	Picture background, frame;
	RectangleShape loseBackground;
	Button newGameButton, scoreBoard, bestScoreBoard, tryAgainButton, backToMenu, back;
	Text scoreTitle, bestScoreTitle, loseTitle;
	pair<int,int> cur;
	Cell cells[7][5], next;

	int score, bestScore;
	bool firstLoad;
	bool isMainMenu;
	bool isGameOver;

	// RenderWindow function: clear, draw, display
	void clear(Color color = Color(0, 0, 0, 255));
	void draw(RectangleShape& shape);
	void draw(Sprite& shape);
	void draw(Picture& picture);
	void draw(Button& button);
	void draw(Text& text);
	void draw(Cell& cell);
	void draw(Cell cell[7][5], int remove_i = -1, int remove_j = -1);
	void display();

	// Main Game function:
	void newGame();
	void update();
	void render();
	void renderText(Text& text, std::string str, Color color, int fontSize, int x, int y);

	// Cells function: move, new, animation
	void makeAnimation(int i, int j, int u, int v);
	void runAnimation(void (Animation::* animate)(RenderWindow*, Picture&));
	void newCells();
	bool moveCells(int move);	// 0 - left, 1 - right, 2 - down	(cur cell)
	bool merge(int u, int v);	// merge cell use recursion
	void handle();				// all handle of board

	bool isLose();
	void saveBestScore();
	void loadBestScore();
	void saveTable();
	void loadTable();
};


#endif

