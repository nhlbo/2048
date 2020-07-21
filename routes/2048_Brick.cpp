#include "..\controls\2048_Brick.h"

void Game::Brick::init(RenderWindow* __window, Resourcepack* __res, Music* __music) {
	window = __window;
	res = __res;
	music = __music;

	firstLoad = true;
	bestScore = 0;
	//loadBestScore();

	for (int i = 0; i < 7; ++i)
		for (int j = 0; j < 5; ++j) {
			cells[i][j].setSize(90, 90);
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

	Cell::setTexture(res->getTexture("block"));
	background.setTexture(res->getTexture("background"));
	frame.setTexture(res->getTexture("frame"));
	frame.setColor(Color(0, 0, 0, 0));

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
		if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) delay = milliseconds(5);

		if (clock.getElapsedTime() > delay) {
			clock.restart();
			if (isGameOver) {
				render();
			}
			else if (cur.first == -1) {		// new cell
				newCells();
			}
			else if (!moveCells(2)) {		// cant move down
				handle();
			}
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
		cur;
		//loadTable();
		//isLose();
	}
	else {
		isGameOver = false;
		score = 0;
		cur = { -1, 2 };
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 5; j++) {
				cells[i][j] = 0;
				copy[i][j] = cells[i][j].getShape();
			}
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

void Game::Brick::makeAnimation(int i, int j, int u, int v) {
	animation.push(cells[i][j].getShape(), cells[i][j].distance(cells[u][v]));
	copy[i][j].setSize(Vector2f(0, 0));						// setSize = (0, 0) to disable this cell
	cells[i][j] = 0;
}

void Game::Brick::runAnimation(void(Animation::* animate)(RenderWindow*, Picture&)) {
	// make window background for animation
	clear(Color::White);
	draw(background);
	draw(newGameButton);
	draw(scoreBoard);
	draw(scoreTitle);
	draw(bestScoreBoard);
	draw(bestScoreTitle);
	for (int i = 0; i < 7; ++i)
		for (int j = 0; j < 5; ++j) {
			if (copy[i][j].getSize() != Vector2f(0, 0)) {	// setSize = (0, 0) to disable this cell
				draw(copy[i][j]);
			}
		}

	// run animation
	(animation.*animate)(window, frame);

	// reset bool checker
	for (int i = 0; i < 7; ++i)
		for (int j = 0; j < 5; ++j) {
			copy[i][j] = cells[i][j].getShape();
		}
}

void Game::Brick::newCells() {
	if (cells[0][2] != 0) return;	// start cell
	cur = { 0, 2 };
	cells[0][2] = 1 << (rand() % 6 + 1);
	animation.push(cells[0][2].getShape(), Vector2f(0, 0));
	runAnimation(&Animation::appear);
}

bool Game::Brick::moveCells(int move) {
	int x = cur.first, y = cur.second;
	if (move == 2) {
		if (x >= 6 || cells[x + 1][y] != 0) return 0;
		cells[x + 1][y] = cells[x][y];
		makeAnimation(x, y, x + 1, y);
		cur.first++;
	}
	if (move == 0) {
		if (y <= 0 || cells[x][y - 1] != 0) return 0;
		cells[x][y - 1] = cells[x][y];
		makeAnimation(x, y, x, y - 1);
		cur.second--;
	}
	if (move == 1){
		if (y >= 4 || cells[x][y + 1] != 0) return 0;
		cells[x][y + 1] = cells[x][y];
		makeAnimation(x, y, x, y + 1);
		cur.second++;
	}
	runAnimation(&Animation::move);
	return 1;
}

bool Game::Brick::merge(int u, int v) {
	if (cells[u][v] == 0) return 0;
	
	// merge cell
	int exp = 0;
	if (v > 0 && cells[u][v - 1] == cells[u][v]) {
		exp++;
		makeAnimation(u, v - 1, u, v);
	}
	if (v < 4 && cells[u][v + 1] == cells[u][v]) {
		exp++;
		makeAnimation(u, v + 1, u, v);
	}
	if (u < 6 && cells[u + 1][v] == cells[u][v]) {
		exp++;
		makeAnimation(u + 1, v, u, v);
	}
	cells[u][v] *= (1 << exp);

	return exp != 0;
}

void Game::Brick::handle() {
	// Queue of merged cells
	int t = 1;
	vector<pair<int, int>> Q[2];
	Time delay = microseconds(1);
	Q[0].push_back(cur);
	
	// moving cell == cell need marge
	while (!Q[t = !t].empty()) {
		// merging cells
		sleep(delay);
		while (!Q[t].empty()) {
			if (merge(Q[t].back().first, Q[t].back().second)) {
				Q[!t].push_back(Q[t].back());
			}
			Q[t].pop_back();
		}
		runAnimation(&Animation::move);
		render();
		
		// falling cells
		sleep(delay);
		for (int j = 0; j < 5; ++j) {
			bool need_fall = 0;
			for (int i = 6; i >= 0; --i)
				if (cells[i][j] == 0) {
					need_fall = 1;
				}
				else if (need_fall) {
					Q[!t].push_back({i + 1, j});
					cells[i + 1][j] = cells[i][j];
					makeAnimation(i, j, i + 1, j);
				}
		}
		runAnimation(&Animation::move);
	}

	cur.first = -1;	// new cell
}

