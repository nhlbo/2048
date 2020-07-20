#include "..\controls\2048_Classic.h"

void Game::Classic::init(RenderWindow* __window, Resourcepack* __res, Music* __music) {
	window = __window;
	res = __res;
	music = __music;

	firstLoad = true;
	bestScore = 0;
	loadBestScore();

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j) {
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
			if (e.type == Event::MouseButtonReleased) {
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
				runAnimation(&Animation::move);
				newCells();
				if (isLose());
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
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
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
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++) {
				cells[i][j] = 0; 
				copy[i][j] = cells[i][j].getShape();
			}
		newCells(2);
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

void Game::Classic::makeAnimation(int i, int j, int u, int v) {
	animation.push(cells[i][j].getShape(), cells[i][j].distance(cells[u][v]));
	copy[i][j].setSize(Vector2f(0, 0));						// setSize = (0, 0) to disable this cell
	cells[i][j] = 0;
}

void Game::Classic::runAnimation(void(Animation::*animate)(RenderWindow*, Picture&)) {
	// make window background for animation
	clear(Color::White);
	draw(background);
	draw(newGameButton);
	draw(scoreBoard);
	draw(scoreTitle);
	draw(bestScoreBoard);
	draw(bestScoreTitle);
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j) {
			if (copy[i][j].getSize() != Vector2f(0, 0)) {	// setSize = (0, 0) to disable this cell
				draw(copy[i][j]);
			}
		}

	// run animation
	(animation.*animate)(window, frame);

	// reset bool checker
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j) {
			copy[i][j] = cells[i][j].getShape();
		}
}

void Game::Classic::newCells(int time) {
	while (1) {
		int emptyCells = 0;
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				emptyCells += (cells[i][j] == 0);
		if (emptyCells == 0) break;

		int randCells = rand() % emptyCells;
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				if (cells[i][j] == 0 && --emptyCells == randCells) {
					cells[i][j] = 1 << (rand() % 2 + 1);
					animation.push(cells[i][j].getShape(), Vector2f(0, 0));
					goto SUCCESS;
				}
		
	SUCCESS:	// break 2 for
		if (--time == 0) break;
	}
	runAnimation(&Animation::appear);
}

bool Game::Classic::moveLeft() {
	//cerr << "Press Left" << endl;
	bool moved = 0;
	for (int j = 1; j < 4; ++j)
		for (int i = 0; i < 4; ++i)
			if (cells[i][j] != 0) {
				for (int k = j - 1; k >= 0; --k) {
					if (cells[i][k] != cells[i][j] && cells[i][k] != 0) {
						cells[i][k + 1] = cells[i][j];
						if (k + 1 != j) {
							makeAnimation(i, j, i, k + 1);
						}
						else break;
					}
					else if (cells[i][k] == cells[i][j]) {
						cells[i][k] += cells[i][j];
						score += cells[i][j].getVal();
						makeAnimation(i, j, i, k);
					}
					else if (k == 0 && cells[i][k] == 0) {
						cells[i][k] = cells[i][j];
						makeAnimation(i, j, i, k);
					}
					else continue;

					moved = 1;
					break;
				}
			}
	return moved;
}

bool Game::Classic::moveRight() {
	//cerr << "Press Right" << endl;
	bool moved = 0;
	for (int j = 4 - 2; j >= 0; --j)
		for (int i = 0; i < 4; ++i)
			if (cells[i][j] != 0) {
				for (int k = j + 1; k < 4; ++k) {
					if (cells[i][k] != cells[i][j] && cells[i][k] != 0) {
						cells[i][k - 1] = cells[i][j];
						if (k - 1 != j) {
							makeAnimation(i, j, i, k - 1);
						}
						else  break;
					}
					else if (cells[i][k] == cells[i][j]) {
						cells[i][k] += cells[i][j];
						score += cells[i][j].getVal();
						makeAnimation(i, j, i, k);
					}
					else if (k == 4 - 1 && cells[i][k] == 0) {
						cells[i][k] = cells[i][j];
						makeAnimation(i, j, i, k);
					}
					else continue;

					moved = 1;
					break;
				}
			}
	return moved;
}

bool Game::Classic::moveUp() {
	//cerr << "Press Up" << endl;
	bool moved = 0;
	for (int i = 1; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (cells[i][j] != 0) {
				for (int k = i - 1; k >= 0; --k) {
					if (cells[k][j] != cells[i][j] && cells[k][j] != 0) {
						cells[k + 1][j] = cells[i][j];
						if (k + 1 != i) {
							makeAnimation(i, j, k + 1, j);
						}
						else  break;
					}
					else if (cells[k][j] == cells[i][j]) {
						cells[k][j] += cells[i][j];
						score += cells[i][j].getVal();
						makeAnimation(i, j, k, j);
					}
					else if (k == 0 && cells[k][j] == 0) {
						cells[k][j] = cells[i][j];
						makeAnimation(i, j, k, j);
					}
					else continue;

					moved = 1;
					break;
				}
			}
	return moved;
}

bool Game::Classic::moveDown() {
	//cerr << "Press Down" << endl;
	bool moved = 0;
	for (int i = 4 - 2; i >= 0; --i)
		for (int j = 0; j < 4; ++j)
			if (cells[i][j] != 0) {
				for (int k = i + 1; k < 4; ++k) {
					if (cells[k][j] != cells[i][j] && cells[k][j] != 0) {
						cells[k - 1][j] = cells[i][j];
						if (k - 1 != i) {
							makeAnimation(i, j, k - 1, j);
						}
						else  break;
					}
					else if (cells[k][j] == cells[i][j]) {
						cells[k][j] += cells[i][j];
						score += cells[i][j].getVal();
						makeAnimation(i, j, k, j);
					}
					else if (k == 4 - 1 && cells[k][j] == 0) {
						cells[k][j] = cells[i][j];
						makeAnimation(i, j, k, j);
					}
					else continue;

					moved = 1;
					break;
				}
			}
	return moved;
}

bool Game::Classic::isLose() {
	int check = -1;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (cells[i][j] == 0)
				return false;
		}
	}

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4 - 1; j++)
			if (cells[i][j] == cells[i][j + 1])
				check++;
	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 4 - 1; i++)
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
	fTemp << 4 << " " << 4 << endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
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
	if (zx == 4 && zy == 4)
		for (int i = 0; i < zx; i++)
			for (int j = 0; j < zy; j++) {
				int x;
				f >> x;
				cells[i][j] = x;
			}
	f.close();
}