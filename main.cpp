#include <SFML/Audio.hpp>
#include "./controls/Game.h"

int main() {
	sf::Music music_background;
	if (!music_background.openFromFile(".\\data\\audio\\background.wav"))
		throw std::runtime_error("Background music file missing");
	music_background.setLoop(1);
	music_background.play();

	Game game;
	game.start();

	music_background.stop();
	return 0;
}