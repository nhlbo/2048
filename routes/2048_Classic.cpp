#include "..\controls\2048_Classic.h"

using namespace sf;
using namespace std;

void Game::Classic::init(RenderWindow* __window, Resourcepack* __res, Music* __music) {
	window = __window;
	res = __res;
	music = __music;

	firstLoad = true;
	bestScore = 0;
	loadBestScore();
	N = 4;	// 4x4

	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j) {
			cells[i][j].setPosition(20 + (150 + 10) * j, 20 + (150 + 10) * i);
			// start position: (20, 20) px
			// size of cells : 150x150 px
			// size of line	 : 10 px
		}
}

bool Game::Classic::loadResourcepack() {
	font.loadFromFile(res->getFont());

	res->setButton(newGameButton, "newgamebutton");
	res->setButton(scoreBoard, "scoreboard");
	res->setButton(bestScoreBoard, "bestscoreboard");
	res->setButton(tryAgainButton, "tryagain");
	res->setButton(backToMenu, "backtomenu");

	Cell::setSize(150, 150);
	Cell::setTexture(res->getTexture("block"));
	background.setTexture(res->getTexture("background"));
	frame.setTexture(res->getTexture("frame"));

	loseBackground.setFillColor(Color(238, 228, 218, 150));
	loseBackground.setPosition(Vector2f(10, 10));
	loseBackground.setSize(Vector2f(650, 650));
	renderText(loseTitle, "Game Over!", Color(119, 110, 101), 78, 145, 195);
	return 1;
}

void Game::Classic::start() {
	newGame();
	while (window->isOpen()) {
		if (music->getStatus() == sf::Sound::Status::Stopped) {
			music->openFromFile(res->getNextSound("music"));
			music->play();
		}
		Event e;
		while (window->pollEvent(e)) {
			if (e.type == Event::Closed || (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)) {
				saveTable();
				window->close();
				return;
			}
			else {
				bool moved = 0;
				if (isGameOver) {
					render();
				}
				if (e.type == Event::KeyPressed) {
					if (e.key.code == Keyboard::Left || e.key.code == Keyboard::A) moved |= moveLeft();
					if (e.key.code == Keyboard::Right || e.key.code == Keyboard::D) moved |= moveRight();
					if (e.key.code == Keyboard::Up || e.key.code == Keyboard::W) moved |= moveUp();
					if (e.key.code == Keyboard::Down || e.key.code == Keyboard::S) moved |= moveDown();
				}
				else if (e.type == Event::MouseButtonReleased) {
					if (newGameButton.clicked(window)) {
						newGame();
					}
					if (isGameOver) {
						if (tryAgainButton.clicked(window)) {
							newGame();
						}
						else if (backToMenu.clicked(window)) {
							saveTable();
							firstLoad = true;
							return;	// back Game::Mainmenu
						}
					}
				}
				if (moved) {
					newCells();
					if (isLose());
				}
			}
		}
	}
}

void Game::Classic::clear(Color color) { window->clear(color); }

void Game::Classic::draw(RectangleShape& shape) { window->draw(shape); }

void Game::Classic::draw(Sprite& shape) { window->draw(shape); }

void Game::Classic::draw(Picture& picture) { picture.draw(window); }

void Game::Classic::draw(Button& button) { button.draw(window); }

void Game::Classic::draw(Text& text) { window->draw(text); }

void Game::Classic::draw(Cell& cell) { cell.draw(window); }

void Game::Classic::draw(Cell cell[4][4], int remove_i, int remove_j) {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			if (i == remove_i && j == remove_j) continue;
			draw(cell[i][j]);
		}
}

void Game::Classic::display() { window->display(); }

void Game::Classic::newGame() {
	if (firstLoad) {
		firstLoad = false;
		loadTable();
		isLose();
	}
	else {
		isGameOver = false;
		score = 0;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++) cells[i][j] = 0;
		newCells();
		newCells();
	}
	update();
	render();
}

void Game::Classic::update() {
	renderText(scoreTitle, to_string(score), Color::White, 15, 565, 165);
	scoreTitle.setPosition(805 - scoreTitle.getLocalBounds().width / 2, 155);
	renderText(bestScoreTitle, to_string(bestScore), Color::White, 15, 565, 235);
	bestScoreTitle.setPosition(805 - bestScoreTitle.getLocalBounds().width / 2, 225);

	if (score > bestScore) {
		bestScore = score;
		saveBestScore();
	}
}

void Game::Classic::render() {
	clear(Color::White);
	draw(background);
	draw(newGameButton);
	draw(scoreBoard);
	draw(scoreTitle);
	draw(bestScoreBoard);
	draw(bestScoreTitle);
	draw(cells);
	draw(frame);

	if (isGameOver) {
		draw(loseBackground);
		draw(loseTitle);
		draw(tryAgainButton);
		draw(backToMenu);
	}

	display();
}

void Game::Classic::renderText(Text& text, string str, Color color, int fontSize, int x, int y) {
	text.setFont(font);
	text.setString(str);
	text.setCharacterSize(fontSize);
	text.setFillColor(color);
	text.setPosition(x, y);
}

void Game::Classic::moving_animation(int i, int j, int u, int v) {
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
		draw(frame);
		display();
	}
}

void Game::Classic::newcell_animation(int u, int v) {
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
		draw(frame);
		display();
	}
}

void Game::Classic::newCells() {
	int emptyCells = 0;
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			emptyCells += (cells[i][j] == 0);

	if (emptyCells == 0) {

		return;
	}
	int randCells = rand() % emptyCells;

	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			if (cells[i][j] == 0 && --emptyCells == randCells) {
				cells[i][j] = 1 << (rand() % 2 + 1);
				newcell_animation(i, j);
				return;
			}
}

bool Game::Classic::moveLeft() {
	//cerr << "Press Left" << endl;
	bool moved = 0;
	for (int j = 1; j < N; ++j)
		for (int i = 0; i < N; ++i)
			if (cells[i][j] != 0) {
				for (int k = j - 1; k >= 0; --k) {
					if (cells[i][k] != cells[i][j] && cells[i][k] != 0) {
						cells[i][k + 1] = cells[i][j];
						if (k + 1 != j) {
							moving_animation(i, k + 1, i, j);
						}
						else break;
					}
					else if (cells[i][k] == cells[i][j]) {
						cells[i][k] += cells[i][j];
						score += cells[i][j].getVal();
						moving_animation(i, k, i, j);
					}
					else if (k == 0 && cells[i][k] == 0) {
						cells[i][k] = cells[i][j];
						moving_animation(i, k, i, j);
					}
					else continue;

					moved |= (cells[i][j] == 0);
					break;
				}
			}
	return moved;
}

bool Game::Classic::moveRight() {
	//cerr << "Press Right" << endl;
	bool moved = 0;
	for (int j = N - 2; j >= 0; --j)
		for (int i = 0; i < N; ++i)
			if (cells[i][j] != 0) {
				for (int k = j + 1; k < N; ++k) {
					if (cells[i][k] != cells[i][j] && cells[i][k] != 0) {
						cells[i][k - 1] = cells[i][j];
						if (k - 1 != j) {
							moving_animation(i, k - 1, i, j);
						}
						else  break;
					}
					else if (cells[i][k] == cells[i][j]) {
						cells[i][k] += cells[i][j];
						score += cells[i][j].getVal();
						moving_animation(i, k, i, j);
					}
					else if (k == N - 1 && cells[i][k] == 0) {
						cells[i][k] = cells[i][j];
						moving_animation(i, k, i, j);
					}
					else continue;

					moved |= (cells[i][j] == 0);
					break;
				}
			}
	return moved;
}

bool Game::Classic::moveUp() {
	//cerr << "Press Up" << endl;
	bool moved = 0;
	for (int i = 1; i < N; ++i)
		for (int j = 0; j < N; ++j)
			if (cells[i][j] != 0) {
				for (int k = i - 1; k >= 0; --k) {
					if (cells[k][j] != cells[i][j] && cells[k][j] != 0) {
						cells[k + 1][j] = cells[i][j];
						if (k + 1 != i) {
							moving_animation(k + 1, j, i, j);
						}
						else  break;
					}
					else if (cells[k][j] == cells[i][j]) {
						cells[k][j] += cells[i][j];
						score += cells[i][j].getVal();
						moving_animation(k, j, i, j);
					}
					else if (k == 0 && cells[k][j] == 0) {
						cells[k][j] = cells[i][j];
						moving_animation(k, j, i, j);
					}
					else continue;

					moved |= (cells[i][j] == 0);
					break;
				}
			}
	return moved;
}

bool Game::Classic::moveDown() {
	//cerr << "Press Down" << endl;
	bool moved = 0;
	for (int i = N - 2; i >= 0; --i)
		for (int j = 0; j < N; ++j)
			if (cells[i][j] != 0) {
				for (int k = i + 1; k < N; ++k) {
					if (cells[k][j] != cells[i][j] && cells[k][j] != 0) {
						cells[k - 1][j] = cells[i][j];
						if (k - 1 != i) {
							moving_animation(k - 1, j, i, j);
						}
						else  break;
					}
					else if (cells[k][j] == cells[i][j]) {
						cells[k][j] += cells[i][j];
						score += cells[i][j].getVal();
						moving_animation(k, j, i, j);
					}
					else if (k == N - 1 && cells[k][j] == 0) {
						cells[k][j] = cells[i][j];
						moving_animation(k, j, i, j);
					}
					else continue;

					moved |= (cells[i][j] == 0);
					break;
				}
			}
	return moved;
}

bool Game::Classic::isLose() {
	int check = -1;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (cells[i][j] == 0)
				return false;
		}
	}

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N - 1; j++)
			if (cells[i][j] == cells[i][j + 1])
				check++;
	for (int j = 0; j < N; j++)
		for (int i = 0; i < N - 1; i++)
			if (cells[i][j] == cells[i + 1][j])
				check++;
	return check < 0 ? isGameOver = true, true : false;
}

void Game::Classic::saveBestScore() {
	fstream f;
	remove("data/best_score.txt");
	f.open("data/best_score.txt", ios::out);
	f << score;
	f.close();
}

void Game::Classic::loadBestScore() {
	int s;
	fstream f;
	f.open("data/best_score.txt", ios::in);
	f >> s;
	f.close();
	s != 0 ? bestScore = s : bestScore = 0;
}

void Game::Classic::saveTable() {
	fstream fTemp;
	fTemp.open("data/temp.txt", ios::out);
	fTemp << N << " " << N << endl;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			fTemp << cells[i][j].getVal() << " ";
		}
		fTemp << endl;
	}
	fTemp.close();
	remove("data/table.txt");
	rename("data/temp.txt", "data/table.txt");
}

void Game::Classic::loadTable() {
	fstream f;
	f.open("data/table.txt");
	if (!f.is_open()) return;
	int zx, zy;
	f >> zx >> zy;
	if (zx == N && zy == N)
		for (int i = 0; i < zx; i++)
			for (int j = 0; j < zy; j++) {
				int x;
				f >> x;
				cells[i][j] = x;
			}
	f.close();
}