#include "..\controls\2048_Brick.h"

using namespace sf;
using namespace std;

void Game::Brick::init(RenderWindow* __window, Resourcepack* __res, Music* __music) {
	window = __window;
	res = __res;
	music = __music;

	firstLoad = true;
	bestScore = 0;
	//loadBestScore();

	for (int i = 0; i < 7; ++i)
		for (int j = 0; j < 5; ++j) {
			cells[i][j].setPosition(110 + 90 * j, 20 + 90 * i);
			// start position: (110, 20) px
			// size of cells : 90x90 px
			// size of line	 : 0 px
		}
}

bool Game::Brick::loadResourcepack() {
	font.loadFromFile(res->getFont());

	res->setButton(newGameButton, "newgamebutton");
	res->setButton(scoreBoard, "scoreboard");
	res->setButton(bestScoreBoard, "bestscoreboard");
	res->setButton(tryAgainButton, "tryagain");
	res->setButton(backToMenu, "backtomenu");

	Cell::setSize(90, 90);
	Cell::setTexture(res->getTexture("block"));
	background.setTexture(res->getTexture("background"));
	frame.setTexture(res->getTexture("frame"));

	loseBackground.setFillColor(Color(238, 228, 218, 150));
	loseBackground.setPosition(Vector2f(10, 10));
	loseBackground.setSize(Vector2f(650, 650));
	renderText(loseTitle, "Game Over!", Color(119, 110, 101), 78, 145, 195);
	return 1;
}

void Game::Brick::start() {
	newGame();
	Clock clock;
	while (window->isOpen()) {
		if (music->getStatus() == sf::Sound::Status::Stopped) {
			music->openFromFile(res->getNextSound("music"));
			music->play();
		}
		Event e;
		Time delay = seconds(1);
		while (window->pollEvent(e)) {
			if (e.type == Event::Closed || (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)) {
				//saveTable();
				window->close();
				return;
			}
			if (e.type == Event::KeyPressed) {
				if (e.key.code == Keyboard::Left || e.key.code == Keyboard::A) moveCells(0);
				if (e.key.code == Keyboard::Right || e.key.code == Keyboard::D) moveCells(1);
			}
			if (cells[cur.first - 1][cur.second] != 0) {

			}
			if (e.type == Event::MouseButtonReleased) {
				if (newGameButton.clicked(window)) {
					newGame();
				}
				if (isGameOver) {
					if (tryAgainButton.clicked(window)) {
						newGame();
					}
					else if (backToMenu.clicked(window)) {
						//saveTable();
						firstLoad = true;
						return;	// back Game::Mainmenu
					}
				}
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) delay = seconds(0.05);

		if (clock.getElapsedTime() > delay) {
			if (isGameOver) {
				render();
			}
			else if (cur.first == -1) {
				newCells();
			}
			else if (!moveCells(2)) {
				merge(cur.first, cur.second);
			}
			
			clock.restart();
		}
	}
}


void Game::Brick::clear(Color color) { window->clear(color); }

void Game::Brick::draw(RectangleShape& shape) { window->draw(shape); }

void Game::Brick::draw(Sprite& shape) { window->draw(shape); }

void Game::Brick::draw(Picture& picture) { picture.draw(window); }

void Game::Brick::draw(Button& button) { button.draw(window); }

void Game::Brick::draw(Text& text) { window->draw(text); }

void Game::Brick::draw(Cell& cell) { cell.draw(window); }

void Game::Brick::draw(Cell cell[7][5], int remove_i, int remove_j) {
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 5; j++) {
			if (i == remove_i && j == remove_j) continue;
			draw(cell[i][j]);
		}
}

void Game::Brick::display() { window->display(); }

void Game::Brick::newGame() {
	if (0 && firstLoad) {
		firstLoad = false;
		cur = { -1, 2 };
		//loadTable();
		//isLose();
	}
	else {
		isGameOver = false;
		score = 0;
		cur = { -1, 2 };
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 5; j++) 
				cells[i][j] = 0;
	}
	update();
	render();
}

void Game::Brick::update() {
	renderText(scoreTitle, to_string(score), Color::White, 15, 565, 165);
	scoreTitle.setPosition(805 - scoreTitle.getLocalBounds().width / 2, 155);
	renderText(bestScoreTitle, to_string(bestScore), Color::White, 15, 565, 235);
	bestScoreTitle.setPosition(805 - bestScoreTitle.getLocalBounds().width / 2, 225);

	if (score > bestScore) {
		bestScore = score;
		//saveBestScore();
	}
}

void Game::Brick::render() {
	clear(Color::White);
	draw(background);
	draw(newGameButton);
	draw(scoreBoard);
	draw(scoreTitle);
	draw(bestScoreBoard);
	draw(bestScoreTitle);
	draw(cells);
	//draw(frame);

	if (isGameOver) {
		draw(loseBackground);
		draw(loseTitle);
		draw(tryAgainButton);
		draw(backToMenu);
	}

	display();
}

void Game::Brick::renderText(Text& text, string str, Color color, int fontSize, int x, int y) {
	text.setFont(font);
	text.setString(str);
	text.setCharacterSize(fontSize);
	text.setFillColor(color);
	text.setPosition(x, y);
}

void Game::Brick::moving_animation(int i, int j, int u, int v) {
	cells[u][v] = 0;
	update();

	// optimize animation
	Texture tex;
	tex.create(950, 720);
	clear();
	draw(background);
	draw(newGameButton);
	draw(scoreBoard);
	draw(scoreTitle);
	draw(bestScoreBoard);
	draw(bestScoreTitle);
	draw(cells);
	tex.update(*window);
	Sprite temp(tex);

	int n_moves = 5;
	RectangleShape cell = cells[i][j].getShape();
	Vector2f posCell = cells[u][v].getShape().getPosition();
	Vector2f posCOld = cell.getPosition();
	Time delay = milliseconds(0.08);

	float dx = posCOld.x - posCell.x;
	float dy = posCOld.y - posCell.y;
	dx /= n_moves; dy /= n_moves;

	for (int move = 0; move < n_moves; ++move) {
		posCell.x += dx;
		posCell.y += dy;
		cell.setPosition(posCell);

		sleep(delay);
		clear();
		draw(temp);
		draw(cell);
		//draw(frame);
		display();
	}
}

void Game::Brick::newcell_animation(int u, int v) {
	// optimize animation
	Texture tex;
	tex.create(950, 720);
	clear();
	draw(background);
	draw(newGameButton);
	draw(scoreBoard);
	draw(scoreTitle);
	draw(bestScoreBoard);
	draw(bestScoreTitle);
	draw(cells, u, v);
	tex.update(*window);
	Sprite temp(tex);

	RectangleShape cell = cells[u][v].getShape();
	Vector2f posCell = cell.getPosition();
	Vector2f size = cell.getSize();
	Time delay = milliseconds(18);

	posCell.x += 25;
	posCell.y += 25;

	for (int i = 50; i >= 0; i -= 10) {
		cell.setSize(Vector2f(size.x - i, size.y - i));
		cell.setPosition(posCell);
		posCell.x -= 5;
		posCell.y -= 5;

		sleep(delay);
		draw(temp);
		draw(cell);
		//draw(frame);
		display();
	}
}

void Game::Brick::newCells() {
	if (cells[0][2] != 0) return;	// start cell
	cur = { 0, 2 };
	cells[0][2] = 1 << (rand() % 6 + 1);
	newcell_animation();
}

bool Game::Brick::moveCells(int move) {
	int x = cur.first, y = cur.second;
	if (move == 2) {
		if (x >= 6 || cells[x + 1][y] != 0) return 0;
		cells[x + 1][y] = cells[x][y];
		moving_animation(x + 1, y, x, y);
		cur.first++;
	}
	if (move == 0) {
		if (y <= 0 || cells[x][y - 1] != 0) return 0;
		cells[x][y - 1] = cells[x][y];
		moving_animation(x, y - 1, x, y);
		cur.second--;
	}
	if (move == 1){
		if (y >= 4 || cells[x][y + 1] != 0) return 0;
		cells[x][y + 1] = cells[x][y];
		moving_animation(x, y + 1, x, y);
		cur.second++;
	}
	return 1;
}

void Game::Brick::merge(int u, int v) {
	cur.first = -1;
	// need animation merge (2-4 cell the same time)
}
