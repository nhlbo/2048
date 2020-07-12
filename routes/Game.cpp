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
			// class::Cell
			cells[i][j].setPosition(coor[j], coor[i]);
			cells[i][j].setPositionText(coorText[j], coorText[i]);
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
		for (int j = 0; j < 4; j++) cells[i][j] = 0;
	newCells();
	newCells();
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
	//this->updateCells();
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
			cells[i][j].draw(window);
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

void Game::moving_animation(int i, int j, int u, int v) {
	cells[u][v] = 0;
	update();

	RectangleShape cell = cells[i][j].getShape();
	Vector2f posCell = cells[u][v].getShape().getPosition();
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
				cells[_i][_j].draw(window);
			}
		}
		
		window->draw(cell);
		window->draw(cells[i][j].getText());
		window->display();
	}
}

void Game::newcell_animation(int u, int v) {
	update();
	RectangleShape cell = cells[u][v].getShape();
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
				cells[_i][_j].draw(window);
			}
		}
		window->draw(cell);
		window->draw(cells[u][v].getText());
		window->display();
	}
}

void Game::newCells() {
	int emptyCells = 0;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			emptyCells += (cells[i][j] == 0);

	if (emptyCells == 0) {

		return;
	}
	int randCells = rand() % emptyCells;

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (cells[i][j] == 0 && --emptyCells == randCells) {
				cells[i][j] = 1 << (rand() % 2 + 1);
				newcell_animation(i, j);
				return;
			}
}

bool Game::moveLeft() {
	cerr << "Press Left" << endl;

	bool moved = 0;
	for (int j = 1; j < 4; ++j) 
		for (int i = 0; i < 4; ++i) 
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
					else if (k == 0 && cells[i][0] == 0) {
						cells[i][0] = cells[i][j];
						moving_animation(i, 0, i, j);
					}
					else continue;

					moved |= (cells[i][j] == 0);
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
			if (cells[i][j] != 0) {
				for (int k = j + 1; k < 4; ++k) {
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
					else if(k == 3 && cells[i][3] == 0) {
						cells[i][3] = cells[i][j];
						moving_animation(i, 3, i, j);
					}
					else continue;

					moved |= (cells[i][j] == 0);
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
					else if (k == 0 && cells[0][j] == 0) {
						cells[0][j] = cells[i][j];
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
	cerr << "Press Down" << endl;

	bool moved = 0;
	for (int i = 2; i >= 0; --i)
		for (int j = 0; j < 4; ++j)
			if (cells[i][j] != 0) {
				for (int k = i + 1; k < 4; ++k) {
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
					else if (k == 3 && cells[3][j] == 0) {
						cells[3][j] = cells[i][j];
						moving_animation(3, j, i, j);
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
	//for (int i = 0; i < 4; i++)
	//	for (int j = 0; j < 3; j++)
	//		if (this->table[i][j] == this->table[i][j + 1])
	//			check++;
	//for (int j = 0; j < 4; j++)
	//	for (int i = 0; i < 3; i++)
	//		if (this->table[i][j] == this->table[i + 1][j])
	//			check++;
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
	//fstream fTemp;
	//fTemp.open("data/temp.txt", ios::out);

	//for (int i = 0; i < 4; i++) {
	//	for (int j = 0; j < 4; j++) {
	//		fTemp << this->table[i][j] << " ";
	//	}
	//	fTemp << endl;
	//}
	//fTemp.close();
	//remove("data/table.txt");
	//rename("data/temp.txt", "data/table.txt");
}

void Game::loadTable() {
	//fstream f;
	//f.open("data/table.txt");
	//if (!f.is_open()) return;
	//for (int i = 0; i < 4; i++)
	//	for (int j = 0; j < 4; j++)
	//		f >> this->table[i][j];
	//f.close();
}