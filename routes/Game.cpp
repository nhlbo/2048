#include "../controls/game.h"

// this->loadBestScore()  <=>  loadBestScore()
// this->window = ...	  <=>  window = ...

Game::Game() {
	window = new RenderWindow(VideoMode(950, 720), "2048", Style::Titlebar | Style::Close);

	m_size_i = 0;	// 0(4x4), 1(6x6), 2(8x8)
	m_size = 4 + 2 * m_size_i;

	// load texture (new add function change size and skin
	font.loadFromFile("data/texture/classical/ClearSans-Bold.ttf");
	skin.loadFromFile("data/texture/classical/background.png");
	skin_b.loadFromFile("data/texture/classical/frame4x4.png");
	newGameButton.setTexture("data/texture/classical/newgame.png");
	scoreBoard.setTexture("data/texture/classical/score.png");
	bestScoreBoard.setTexture("data/texture/classical/bestscore.png");
	
	Cell::setSize(size[m_size_i][2], size[m_size_i][2]);
	firstLoad = true;
	bestScore = 0;
	srand(time(NULL));
	loadBestScore();

	// add texture to m_member
	background.setTexture(skin);
	frame.setTexture(skin_b);
	newGameButton.setSize(110, 32);
	newGameButton.setPosition(710, 70);
	scoreBoard.setSize(110, 50);
	scoreBoard.setPosition(710, 130);
	bestScoreBoard.setSize(110, 50);
	bestScoreBoard.setPosition(710, 200);
	
	for (int i = 0; i < m_size; i++) {
		for (int j = 0; j < m_size; j++) {
			cells[i][j].setPosition(size[m_size_i][0] + (size[m_size_i][2] + LINE_SIZE) * j, size[m_size_i][1] + (size[m_size_i][2] + LINE_SIZE) * i);
		}
	}
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
			if (e.type == Event::Closed || (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)) {
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
				if (newGameButton.clicked(window)) {
					newGame();
				}
			}
			if (moved) {
				for (int i = 1; i <= m_size_i + 1; ++i)
					newCells();
			}
			
		}
	}
}

void Game::clear(Color color) { window->clear(color); }
void Game::draw(RectangleShape& shape) { window->draw(shape); }
void Game::draw(Sprite& shape) { window->draw(shape); }
void Game::draw(Button& button) { button.draw(window); }
void Game::draw(Text& text) { window->draw(text); }
void Game::draw(Cell& cell){ cell.draw(window); }
void Game::display() { window->display(); }

void Game::newGame() {
	this->score = 0;
	for (int i = 0; i < m_size; i++)
		for (int j = 0; j < m_size; j++) cells[i][j] = 0;
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
	this->scoreTitle.setPosition(765 - this->scoreTitle.getLocalBounds().width / 2, 155);
	this->renderText(this->bestScoreTitle, to_string(this->bestScore), Color::White, 15, 565, 235);
	this->bestScoreTitle.setPosition(765 - this->bestScoreTitle.getLocalBounds().width / 2, 225);

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
	Vector2f posCOld= cell.getPosition();
	Time delay = milliseconds(0.08);

	float dx = posCOld.x - posCell.x;
	float dy = posCOld.y - posCell.y;
	dx /= n_moves; dy /= n_moves;

	for (int move = 0; move < n_moves; ++move){
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
					else if(k == m_size - 1 && cells[i][k] == 0) {
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