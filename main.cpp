#include ".\controls\Resourcepack.h"
#include ".\controls\2048_Brick.h"
#include ".\controls\2048_Classic.h"

using namespace sf;

int main() {
	srand(time(NULL));
	RenderWindow window(VideoMode(950, 720), "2048", Style::Titlebar | Style::Close);
	Resourcepack res;
	Music music;
	
	G2048::Skin skin;
	G2048::Classic classic;
	G2048::Brick brick;

	Picture background;
	Button classicButton, brickButton, resouButton;
	bool isMainmenu = 0;

	// Game::initialization
	skin.init(&window, &res, &music);
	classic.init(&window, &res, &music);
	brick.init(&window, &res, &music);

	// Game::loadResourcepack
	classic.loadResourcepack();
	brick.loadResourcepack();
	background.setTexture(res.getTexture("mainmenu"));
	res.setButton(classicButton, "classic");
	res.setButton(brickButton, "brick");
	res.setButton(resouButton, "newskin");

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
		}

		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed || (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)) {
				window.close();
			}
			else if (e.type == Event::MouseButtonReleased) {
				if (classicButton.clicked(&window)) {
					// play 2048::classic
					classic.start();
				}
				else if (brickButton.clicked(&window)) {
					// play 2048::brick
					brick.start();
				}
				else if (resouButton.clicked(&window)) {
					// make window background
					window.clear();
					background.draw(&window);
					classicButton.draw(&window);
					brickButton.draw(&window);
					resouButton.draw(&window);
					// change resourcepack
					skin.open();
					// Game::loadResourcepack
					classic.loadResourcepack();
					brick.loadResourcepack();
					background.setTexture(res.getTexture("mainmenu"));
					res.setButton(classicButton, "classic");
					res.setButton(brickButton, "brick");
					res.setButton(resouButton, "newskin");
				}
				else continue;

				isMainmenu = 0;
			}
		}
	}
	music.stop();
	return 0;
}