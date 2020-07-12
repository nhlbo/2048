#include "../controls/game.h"

// this->loadBestScore()  <=>  loadBestScore()
// this->window = ...	  <=>  window = ...

Game::Game() {
	firstLoad = true;
	bestScore = 0;
	srand(time(NULL));
	this->loadBestScore();
	font.loadFromFile("data/ClearSans-Bold.ttf");
	this->window = new RenderWindow(VideoMode(670, 470), "2048", Style::Titlebar | Style::Close);
	this->backgroundTable.setSize(Vector2f(450, 450));
	this->backgroundTable.setFillColor(Color(179, 164, 151));
	this->backgroundTable.setPosition(10, 10);
	this->newGameButton.setSize(Vector2f(110, 30));
	this->newGameButton.setFillColor(Color(130, 111, 94));
	this->newGameButton.setPosition(510, 70);
	this->scoreBoard.setSize(Vector2f(110, 50));
	this->scoreBoard.setFillColor(Color(177, 164, 152));
	this->scoreBoard.setPosition(510, 130);
	this->bestScoreBoard.setSize(Vector2f(110, 50));
	this->bestScoreBoard.setFillColor(Color(177, 164, 152));
	this->bestScoreBoard.setPosition(510, 200);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->cellsColor[i][j].setSize(Vector2f(100, 100));
			this->cellsColor[i][j].setPosition(this->coor[j], this->coor[i]);
		}
	}
	this->renderText(this->newGameTitle, "New Game", Color::White, 15, 527, 75);
	this->renderText(this->scoreBoardTitle, "Score", Color::White, 15, 565, 135);
	this->scoreBoardTitle.setPosition(565 - this->scoreBoardTitle.getLocalBounds().width / 2, 135);
	this->renderText(this->bestScoreBoardTitle, "Best Score", Color::White, 15, 565, 135);
	this->bestScoreBoardTitle.setPosition(565 - this->bestScoreBoardTitle.getLocalBounds().width / 2, 205);
}

Game::~Game() {
	delete this->window;
}

void Game::start() {
	this->newGame();
	while (this->window->isOpen()) {
		Event e;
		while (window->pollEvent(e)) {
			bool moved = 0;
			if (e.type == Event::Closed) {
				this->saveTable();
				window->close();
			}
			else if (e.type == Event::KeyPressed) {
				if (e.key.code == Keyboard::Left || e.key.code == Keyboard::A) moved |= this->moveLeft();
				if (e.key.code == Keyboard::Right || e.key.code == Keyboard::D) moved |= this->moveRight();
				if (e.key.code == Keyboard::Up || e.key.code == Keyboard::W) moved |= this->moveUp();
				if (e.key.code == Keyboard::Down || e.key.code == Keyboard::S) moved |= this->moveDown();
			}
			else if (e.type == Event::MouseButtonReleased) {
				pair <int, int> coorMouse = make_pair(Mouse::getPosition(*this->window).x, Mouse::getPosition(*this->window).y);
				if (510 <= coorMouse.first && coorMouse.first <= 620
					&& 70 <= coorMouse.second && coorMouse.second <= 100) {
					this->newGame();
				}
			}
			if (moved) {
				newCells();
			}
			
		}
		//this->update();
		//this->render();
	}
}

void Game::newGame() {
	this->score = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) this->table[i][j] = 0;
	for (int i = 0; i < 2; i++) {
		while (true) {
			int x = rand() % 4, y = rand() % 4;
			if (this->table[x][y] != 0) continue;
			else {
				this->table[x][y] = 1 << (rand() % 2 + 1);
				break;
			}
		}
	}
	if (this->firstLoad) {
		this->firstLoad = false;
		this->loadTable();
	}
	this->update();
	this->render();
}

void Game::update() {
	this->renderText(this->scoreTitle, to_string(this->score), Color::White, 15, 565, 165);
	this->scoreTitle.setPosition(565 - this->scoreTitle.getLocalBounds().width / 2, 155);
	this->renderText(this->bestScoreTitle, to_string(this->bestScore), Color::White, 15, 565, 235);
	this->bestScoreTitle.setPosition(565 - this->bestScoreTitle.getLocalBounds().width / 2, 225);
	this->updateCells();
	if (score > bestScore) {
		bestScore = score;
		this->saveBestScore();
	}
}

void Game::render() {
	this->window->clear(Color::White);
	this->window->draw(backgroundTable);
	this->window->draw(newGameButton);
	this->window->draw(this->newGameTitle);
	this->window->draw(this->scoreBoard);
	this->window->draw(this->scoreBoardTitle);
	this->window->draw(this->scoreTitle);
	this->window->draw(this->bestScoreBoard);
	this->window->draw(this->bestScoreBoardTitle);
	this->window->draw(this->bestScoreTitle);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->window->draw(cellsColor[i][j]);
			this->window->draw(cellsText[i][j]);
		}
	}
	this->window->display();
}

void Game::renderText(Text& text, string str, Color color, int fontSize, int x, int y) {
	text.setFont(this->font);
	text.setString(str);
	text.setCharacterSize(fontSize);
	text.setFillColor(color);
	text.setPosition(x, y);
}

string Game::pointToString(int point) {
	return (point == 0 ? "" : to_string(point));
}

int Game::getFontSize(int point) {
	int cnt = 0;
	while (point > 0) {
		point /= 10;
		cnt++;
	}
	if (cnt == 1) return 40;
	else if (cnt == 2) return 40;
	else if (cnt == 3) return 35;
	else if (cnt == 4) return 30;
	else if (cnt == 5) return 25;
	else if (cnt == 6) return 20;
	return 0;
}

Color Game::getCellColor(int val) {
	Color res;
	if (val == 0) res = Color(198, 184, 171);
	else if (val == 2) res = Color(236, 224, 214);
	else if (val == 4) res = Color(235, 220, 195);
	else if (val == 8) res = Color(243, 168, 115);
	else if (val == 16) res = Color(248, 139, 94);
	else if (val == 32) res = Color(250, 113, 90);
	else if (val == 64) res = Color(251, 85, 60);
	else if (val == 128) res = Color(236, 201, 111);
	else if (val == 256) res = Color(236, 198, 97);
	else if (val == 512) res = Color(237, 194, 83);
	else if (val == 1024) res = Color(237, 191, 71);
	else if (val == 2048) res = Color(239, 187, 57);
	else if (val == 4096) res = Color(240, 92, 97);
	else if (val == 8192) res = Color(246, 67, 84);
	else if (val == 16384) res = Color(249, 57, 62);
	else if (val == 32768) res = Color(99, 168, 203);
	else if (val == 65536) res = Color(73, 147, 212);
	else if (val == 131072) res = Color(0, 116, 192);
	return res;
}

void Game::updateCells() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->cellsColor[i][j].setFillColor(this->getCellColor(this->table[i][j]));
			this->renderText(this->cellsText[i][j], pointToString(this->table[i][j]), (this->table[i][j] <= 4 ? Color(108, 99, 91) : Color::White), this->getFontSize(table[i][j]), this->coor[j], this->coor[i]);
			this->cellsText[i][j].setPosition(this->coorText[j] - this->cellsText[i][j].getLocalBounds().width / 2, this->coorText[i] - this->cellsText[i][j].getLocalBounds().height);
		}
	}
}

void Game::moving_animation(int i, int j, int u, int v) {
	table[u][v] = 0;
	update();

	RectangleShape cell = cellsColor[i][j];
	Vector2f posCell = cellsColor[u][v].getPosition();
	Vector2f posCOld= cell.getPosition();
	Time delay = milliseconds(0.08);

	while (posCell != posCOld) {
		if (posCell.x != posCOld.x) {
			posCell.x += (posCell.x < posCOld.x) ? 11 : -11;
		}
		if (posCell.y != posCOld.y) {
			posCell.y += (posCell.y < posCOld.y) ? 11 : -11;
		}
		cell.setPosition(posCell);

		sleep(delay);
		window->clear(Color::White);
		window->draw(backgroundTable);
		window->draw(newGameButton);
		window->draw(newGameTitle);
		window->draw(scoreBoard);
		window->draw(scoreBoardTitle);
		window->draw(scoreTitle);
		window->draw(bestScoreBoard);
		window->draw(bestScoreBoardTitle);
		window->draw(bestScoreTitle);
		for (int _i = 0; _i < 4; _i++) {
			for (int _j = 0; _j < 4; _j++) {
				if (_i == i && _j == j) continue;
				window->draw(cellsColor[_i][_j]);
				window->draw(cellsText[_i][_j]);
			}
		}
		
		window->draw(cell);
		window->draw(cellsText[i][j]);
		window->display();
	}
}

void Game::newcell_animation(int u, int v) {
	update();
	RectangleShape cell = cellsColor[u][v];
	Vector2f posCell = cell.getPosition();
	Time delay = milliseconds(18);

	posCell.x += 25;
	posCell.y += 25;

	for (int i = 50; i >= 0; i -= 10) {
		cell.setSize(Vector2f(100 - i, 100 - i));
		cell.setPosition(posCell);
		posCell.x -= 5;
		posCell.y -= 5;

		sleep(delay);
		window->clear(Color::White);
		window->draw(backgroundTable);
		window->draw(newGameButton);
		window->draw(newGameTitle);
		window->draw(scoreBoard);
		window->draw(scoreBoardTitle);
		window->draw(scoreTitle);
		window->draw(bestScoreBoard);
		window->draw(bestScoreBoardTitle);
		window->draw(bestScoreTitle);
		for (int _i = 0; _i < 4; _i++) {
			for (int _j = 0; _j < 4; _j++) {
				if (_i == u && _j == v) continue;
				window->draw(cellsColor[_i][_j]);
				window->draw(cellsText[_i][_j]);
			}
		}
		window->draw(cell);
		window->draw(cellsText[u][v]);
		window->display();
	}
}

void Game::newCells() {
	int emptyCells = 0;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			emptyCells += (table[i][j] == 0);

	if (emptyCells == 0) {

		return;
	}
	int randCells = rand() % emptyCells;

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (table[i][j] == 0 && --emptyCells == randCells) {
				table[i][j] = 1 << (rand() % 2 + 1);
				newcell_animation(i, j);
				return;
			}
}

bool Game::moveLeft() {
	cerr << "Press Left" << endl;

	bool moved = 0;
	for (int j = 1; j < 4; ++j) 
		for (int i = 0; i < 4; ++i) 
			if (table[i][j] != 0) {
				for (int k = j - 1; k >= 0; --k) {
					if (table[i][k] != table[i][j] && table[i][k]) {
						table[i][k + 1] = table[i][j];
						if (k + 1 != j) {
							moving_animation(i, k + 1, i, j);
						}
						else break;
					}
					else if (table[i][k] == table[i][j]) {
						table[i][k] += table[i][j];
						score += table[i][j];
						moving_animation(i, k, i, j);
					}
					else if (k == 0 && table[i][0] == 0) {
						table[i][0] = table[i][j];
						moving_animation(i, 0, i, j);
					}
					else continue;

					moved |= (table[i][j] == 0);
					break;
				}
			}
	return moved;
}

bool Game::moveRight() {
	cerr << "Press Right" << endl;

	bool moved = 0;
	for (int j = 2; j >= 0; --j)
		for (int i = 0; i < 4; ++i)
			if (table[i][j] != 0) {
				for (int k = j + 1; k < 4; ++k) {
					if (table[i][k] != table[i][j] && table[i][k]) {
						table[i][k - 1] = table[i][j];
						if (k - 1 != j) {
							moving_animation(i, k - 1, i, j);
						}
						else  break;
					} 
					else if (table[i][k] == table[i][j]) {
						table[i][k] += table[i][j];
						score += table[i][j];
						moving_animation(i, k, i, j);
					}
					else if(k == 3 && table[i][3] == 0) {
						table[i][3] = table[i][j];
						moving_animation(i, 3, i, j);
					}
					else continue;

					moved |= (table[i][j] == 0);
					break;
				}
			}
	return moved;
}

bool Game::moveUp() {
	cerr << "Press Up" << endl;

	bool moved = 0;
	for (int i = 1; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (table[i][j] != 0) {
				for (int k = i - 1; k >= 0; --k) {
					if (table[k][j] != table[i][j] && table[k][j]) {
						table[k + 1][j] = table[i][j];
						if (k + 1 != i) {
							moving_animation(k + 1, j, i, j);
						}
						else  break;
					}
					else if (table[k][j] == table[i][j]) {
						table[k][j] += table[i][j];
						score += table[i][j];
						moving_animation(k, j, i, j);
					}
					else if (k == 0 && table[0][j] == 0) {
						table[0][j] = table[i][j];
						moving_animation(k, j, i, j);
					}
					else continue;

					moved |= (table[i][j] == 0);
					break;
				}
			}
	return moved;
}

bool Game::moveDown() {
	cerr << "Press Down" << endl;

	bool moved = 0;
	for (int i = 2; i >= 0; --i)
		for (int j = 0; j < 4; ++j)
			if (table[i][j] != 0) {
				for (int k = i + 1; k < 4; ++k) {
					if (table[k][j] != table[i][j] && table[k][j]) {
						table[k - 1][j] = table[i][j];
						if (k - 1 != i) {
							moving_animation(k - 1, j, i, j);
						}
						else  break;
					}
					else if (table[k][j] == table[i][j]) {
						table[k][j] += table[i][j];
						score += table[i][j];
						moving_animation(k, j, i, j);
					}
					else if (k == 3 && table[3][j] == 0) {
						table[3][j] = table[i][j];
						moving_animation(3, j, i, j);
					}
					else continue;

					moved |= (table[i][j] == 0);
					break;
				}
			}
	return moved;
}

bool Game::isLose() {
	int check = -1;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 3; j++)
			if (this->table[i][j] == this->table[i][j + 1])
				check++;
	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 3; i++)
			if (this->table[i][j] == this->table[i + 1][j])
				check++;
	return check < 0;
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

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			fTemp << this->table[i][j] << " ";
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
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			f >> this->table[i][j];
	f.close();
}