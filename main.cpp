#include ".\controls\2048_Brick.h"
#include ".\controls\2048_Classic.h"

using namespace sf;

int main() {
	RenderWindow window(VideoMode(950, 720), "2048", Style::Titlebar | Style::Close);
	Resourcepack res;
	Music music;
	Picture background;
	Button classicButton, brickButton, resouButton;
	Game::Classic classic;
	Game::Brick brick;
	bool isMainmenu = 0;

	// Game::initialization
	res.setResourcepack("classical");
	music.openFromFile(res.getNextSound("music"));
	background.setTexture(res.getTexture("mainmenu"));
	res.setButton(classicButton, "classicbutton");
	res.setButton(brickButton, "brickbutton");
	res.setButton(resouButton, "resoubutton");
	classic.init(&window, &res, &music);
	brick.init(&window, &res, &music);

	// Game::start 
	music.play();
	while (window.isOpen()) {
		if (music.getStatus() == sf::Sound::Status::Stopped) {
			music.openFromFile(res.getNextSound("music"));
			music.play();
		}
		if (isMainmenu == 0) {
			window.clear();
			background.draw(&window);
			classicButton.draw(&window);
			brickButton.draw(&window);
			resouButton.draw(&window);
			window.display();
			isMainmenu = 1;
			classic.loadResourcepack();
			//brick.loadResourcepack();
		}

		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed || (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)) {
				window.close();
				music.stop();
			}
			else if (e.type == Event::MouseButtonReleased) {
				if (classicButton.clicked(&window)) {
					// play 2048::classic
					classic.start();
				}
				else if (brickButton.clicked(&window)) {
					// play 2048::brick
				}
				else if (resouButton.clicked(&window)) {
					// change resourcepack
				}
				else continue;

				isMainmenu = 0;
			}
		}
	}
	return 0;
}