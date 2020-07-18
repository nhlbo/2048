#include "../controls/game.h"

// this->loadBestScore()  <=>  loadBestScore()
// this->window = ...	  <=>  window = ...

Game::Game() {
	window = new RenderWindow(VideoMode(950, 720), "2048", Style::Titlebar | Style::Close);

	/*m_size_i = 0;	// 0(4x4), 1(6x6), 2(8x8)
	m_size = 4 + 2 * m_size_i;*/

	isMainMenu = true;
	firstLoad = true;
	bestScore = 0;
	srand(time(NULL));
	loadBestScore();

	/*for (int i = 0; i < m_size; i++) {
		for (int j = 0; j < m_size; j++) {
			cells[i][j].setPosition(size[m_size_i][0] + (size[m_size_i][2] + LINE_SIZE) * j, size[m_size_i][1] + (size[m_size_i][2] + LINE_SIZE) * i);
		}
	}*/
}

Game::~Game() {
	delete this->window;
}

void Game::start() {
	if (loadResourcepack() == 0) {
		window->close();
		return;
	}
	music_background.play();
	while (this->window->isOpen()) {
		if (music_background.getStatus() == sf::Sound::Status::Stopped) {
			music_background.openFromFile(res.getNextSound("music"));
			music_background.play();
		}
		Event e;
		while (window->pollEvent(e)) {
			if (e.type == Event::Closed || (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)) {
				this->saveTable();
				window->close();
				music_background.stop();
			}
			else if (isMainMenu) {
				draw(mainmenu);
				draw(_4x4);
				draw(_6x6);
				draw(_8x8);
				display();
				if (e.type == Event::MouseButtonReleased) {
					/*if (newGameButton.clicked(window)) {
						isMainMenu = false;
						newGame();
					}*/
					if (_4x4.clicked(window)) {
						isMainMenu = false;
						setSize(0);
						newGame();
					}
					else if (_6x6.clicked(window)) {
						isMainMenu = false;
						setSize(1);
						newGame();
					}
					else if (_8x8.clicked(window)) {
						isMainMenu = false;
						setSize(2);
						newGame();
					}
				}
			}
			else {
				bool moved = 0;
				if (isGameOver) {
					render();
				}
				if (e.type == Event::KeyPressed) {
					if (e.key.code == Keyboard::Left || e.key.code == Keyboard::A) moved |= this->moveLeft();
					if (e.key.code == Keyboard::Right || e.key.code == Keyboard::D) moved |= this->moveRight();
					if (e.key.code == Keyboard::Up || e.key.code == Keyboard::W) moved |= this->moveUp();
					if (e.key.code == Keyboard::Down || e.key.code == Keyboard::S) moved |= this->moveDown();
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
							this->saveTable();
							isMainMenu = true;
							break;
						}
					}
				}
				if (moved) {
					for (int i = 1; i <= m_size_i + 1; ++i)
						newCells();
					if (isLose()) {
						cout << "You Lose" << endl;
					}
				}
			}
		}
	}
}

bool Game::loadResourcepack(const char* pack_name) {
	if (!res.setResourcepack(pack_name)) return 0;

	music_background.openFromFile(res.getNextSound("music"));
	font.loadFromFile(res.getFont("score"));

	res.setButton(newGameButton, "newgamebutton");
	res.setButton(scoreBoard, "scoreboard");
	res.setButton(bestScoreBoard, "bestscoreboard");
	res.setButton(tryAgainButton, "tryagain");
	res.setButton(backToMenu, "backtomenu");
	res.setButton(_4x4, "4x4");
	res.setButton(_6x6, "6x6");
	res.setButton(_8x8, "8x8");
	//res.setCell("4x4");

	skin_0.loadFromFile(res.getTexture("mainmenu"));
	skin.loadFromFile(res.getTexture("background"));
	//skin_b.loadFromFile(res.getTexture("frame", "4x4"));

	mainmenu.setTexture(skin_0);
	background.setTexture(skin);
	//frame.setTexture(skin_b);

	loseBackground.setFillColor(sf::Color(238, 228, 218, 150));
	loseBackground.setPosition(Vector2f(10, 10));
	loseBackground.setSize(Vector2f(650, 650));
	this->renderText(this->loseTitle, "Game Over!", Color(119, 110, 101), 78, 145, 195);

	return 1;
}

void Game::clear(Color color) { window->clear(color); }
void Game::draw(RectangleShape& shape) { window->draw(shape); }
void Game::draw(Sprite& shape) { window->draw(shape); }
void Game::draw(Button& button) { button.draw(window); }
void Game::draw(Text& text) { window->draw(text); }
void Game::draw(Cell& cell) { cell.draw(window); }
void Game::display() { window->display(); }

void Game::setSize(int s) {
	res.setCell("4x4");
	skin_b.loadFromFile(res.getTexture("frame", "4x4"));
	frame.setTexture(skin_b);

	switch (s) {
	case 0:
		m_size_i = 0;
		res.setCell("4x4");
		skin_b.loadFromFile(res.getTexture("frame", "4x4"));
		frame.setTexture(skin_b);
		break;
	case 1:
		m_size_i = 1;
		res.setCell("6x6");
		skin_b.loadFromFile(res.getTexture("frame", "6x6"));
		frame.setTexture(skin_b);
		break;
	case 2:
		m_size_i = 2;
		res.setCell("8x8");
		skin_b.loadFromFile(res.getTexture("frame", "8x8"));
		frame.setTexture(skin_b);
		break;
	}


	m_size = 4 + 2 * m_size_i;
	for (int i = 0; i < m_size; i++) {
		for (int j = 0; j < m_size; j++) {
			cells[i][j].setPosition(size[this->m_size_i][0] + (size[m_size_i][2] + LINE_SIZE) * j, size[m_size_i][1] + (size[m_size_i][2] + LINE_SIZE) * i);
		}
	}
}

void Game::newGame() {
	this->isGameOver = false;
	this->score = 0;
	for (int i = 0; i < m_size; i++)
		for (int j = 0; j < m_size; j++) cells[i][j] = 0;
	newCells();
	newCells();
	if (this->firstLoad) {
		this->firstLoad = false;
		this->loadTable();
		isLose();
	}
	this->update();
	this->render();
}

void Game::update() {
	this->renderText(this->scoreTitle, to_string(this->score), Color::White, 15, 565, 165);
	this->scoreTitle.setPosition(805 - this->scoreTitle.getLocalBounds().width / 2, 155);
	this->renderText(this->bestScoreTitle, to_string(this->bestScore), Color::White, 15, 565, 235);
	this->bestScoreTitle.setPosition(805 - this->bestScoreTitle.getLocalBounds().width / 2, 225);

	if (score > bestScore) {
		bestScore = score;
		this->saveBestScore();
	}
}

void Game::render() {
	clear(Color::White);
	draw(background);
	draw(newGameButton);
	draw(scoreBoard);
	draw(scoreTitle);
	draw(bestScoreBoard);
	draw(bestScoreTitle);

	for (int i = 0; i < m_size; i++) {
		for (int j = 0; j < m_size; j++) {
			draw(cells[i][j]);
		}
	}
	draw(frame);

	if (isGameOver) {
		draw(loseBackground);
		draw(loseTitle);
		draw(tryAgainButton);
		draw(backToMenu);
	}

	display();
}

void Game::renderText(Text& text, string str, Color color, int fontSize, int x, int y) {
	text.setFont(this->font);
	text.setString(str);
	text.setCharacterSize(fontSize);
	text.setFillColor(color);
	text.setPosition(x, y);
}

void Game::moving_animation(int i, int j, int u, int v) {
	cells[u][v] = 0;
	update();

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
		draw(background);
		draw(newGameButton);
		draw(scoreBoard);
		draw(scoreTitle);
		draw(bestScoreBoard);
		draw(bestScoreTitle);

		for (int _i = 0; _i < m_size; _i++) {
			for (int _j = 0; _j < m_size; _j++) {
				if (_i == i && _j == j) continue;
				draw(cells[_i][_j]);
			}
		}

		draw(cell);
		draw(frame);
		display();
	}
}

void Game::newcell_animation(int u, int v) {
	RectangleShape cell = cells[u][v].getShape();
	Vector2f posCell = cell.getPosition();
	Time delay = milliseconds(18);

	posCell.x += 25;
	posCell.y += 25;

	for (int i = 50; i >= 0; i -= 10) {
		cell.setSize(Vector2f(size[m_size_i][2] - i, size[m_size_i][2] - i));
		cell.setPosition(posCell);
		posCell.x -= 5;
		posCell.y -= 5;

		sleep(delay);
		clear(Color::White);
		draw(background);
		draw(newGameButton);
		draw(scoreBoard);
		draw(scoreTitle);
		draw(bestScoreBoard);
		draw(bestScoreTitle);

		for (int _i = 0; _i < m_size; _i++) {
			for (int _j = 0; _j < m_size; _j++) {
				if (_i == u && _j == v) continue;
				draw(cells[_i][_j]);
			}
		}

		draw(cell);
		draw(frame);
		display();
	}
}

void Game::newCells() {
	int emptyCells = 0;
	for (int i = 0; i < m_size; ++i)
		for (int j = 0; j < m_size; ++j)
			emptyCells += (cells[i][j] == 0);

	if (emptyCells == 0) {

		return;
	}
	int randCells = rand() % emptyCells;

	for (int i = 0; i < m_size; ++i)
		for (int j = 0; j < m_size; ++j)
			if (cells[i][j] == 0 && --emptyCells == randCells) {
				cells[i][j] = 1 << (rand() % 2 + 1);
				newcell_animation(i, j);
				return;
			}
}

bool Game::moveLeft() {
	//cerr << "Press Left" << endl;
	bool moved = 0;
	for (int j = 1; j < m_size; ++j)
		for (int i = 0; i < m_size; ++i)
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
						score += cells[i][j].getVal() / (1 << m_size_i);	// bigger is small score
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

bool Game::moveRight() {
	//cerr << "Press Right" << endl;
	bool moved = 0;
	for (int j = m_size - 2; j >= 0; --j)
		for (int i = 0; i < m_size; ++i)
			if (cells[i][j] != 0) {
				for (int k = j + 1; k < m_size; ++k) {
					if (cells[i][k] != cells[i][j] && cells[i][k] != 0) {
						cells[i][k - 1] = cells[i][j];
						if (k - 1 != j) {
							moving_animation(i, k - 1, i, j);
						}
						else  break;
					}
					else if (cells[i][k] == cells[i][j]) {
						cells[i][k] += cells[i][j];
						score += cells[i][j].getVal() / (1 << m_size_i);	// bigger is small score
						moving_animation(i, k, i, j);
					}
					else if (k == m_size - 1 && cells[i][k] == 0) {
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

bool Game::moveUp() {
	//cerr << "Press Up" << endl;
	bool moved = 0;
	for (int i = 1; i < m_size; ++i)
		for (int j = 0; j < m_size; ++j)
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
						score += cells[i][j].getVal() / (1 << m_size_i);	// bigger is small score
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

bool Game::moveDown() {
	//cerr << "Press Down" << endl;
	bool moved = 0;
	for (int i = m_size - 2; i >= 0; --i)
		for (int j = 0; j < m_size; ++j)
			if (cells[i][j] != 0) {
				for (int k = i + 1; k < m_size; ++k) {
					if (cells[k][j] != cells[i][j] && cells[k][j] != 0) {
						cells[k - 1][j] = cells[i][j];
						if (k - 1 != i) {
							moving_animation(k - 1, j, i, j);
						}
						else  break;
					}
					else if (cells[k][j] == cells[i][j]) {
						cells[k][j] += cells[i][j];
						score += cells[i][j].getVal() / (1 << m_size_i);	// bigger is small score
						moving_animation(k, j, i, j);
					}
					else if (k == m_size - 1 && cells[k][j] == 0) {
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

bool Game::isLose() {
	int check = -1;

	for (int i = 0; i < m_size; i++) {
		for (int j = 0; j < m_size; j++) {
			if (cells[i][j] == 0)
				return false;
		}
	}

	for (int i = 0; i < m_size; i++)
		for (int j = 0; j < m_size - 1; j++)
			if (cells[i][j] == cells[i][j + 1])
				check++;
	for (int j = 0; j < m_size; j++)
		for (int i = 0; i < m_size - 1; i++)
			if (cells[i][j] == cells[i + 1][j])
				check++;
	return check < 0 ? isGameOver = true, true : false;
}

void Game::saveBestScore() {
	fstream f;
	remove("data/best_score.txt");
	f.open("data/best_score.txt", ios::out);
	f << this->score;
	f.close();
}

void Game::loadBestScore() {
	int s;
	fstream f;
	f.open("data/best_score.txt", ios::in);
	f >> s;
	f.close();
	s != 0 ? this->bestScore = s : this->bestScore = 0;
}

void Game::saveTable() {
	fstream fTemp;
	fTemp.open("data/temp.txt", ios::out);
	fTemp << m_size << " " << m_size << endl;
	for (int i = 0; i < m_size; i++) {
		for (int j = 0; j < m_size; j++) {
			fTemp << cells[i][j].getVal() << " ";
		}
		fTemp << endl;
	}
	fTemp.close();
	remove("data/table.txt");
	rename("data/temp.txt", "data/table.txt");
}

void Game::loadTable() {
	fstream f;
	f.open("data/table.txt");
	if (!f.is_open()) return;
	int zx, zy;
	f >> zx >> zy;
	if (zx == m_size && zy == m_size)
		for (int i = 0; i < zx; i++)
			for (int j = 0; j < zy; j++) {
				int x;
				f >> x;
				cells[i][j] = x;
			}
	f.close();
}