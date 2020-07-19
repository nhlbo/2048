#ifndef _2048_BRICK_H
#define _2048_BRICK_H
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "Resourcepack.h"
#include "Button.h"
#include "Cell.h"
#include "Picture.h"

namespace Game { class Brick; }

class Game::Brick {
public:
	void init(sf::RenderWindow* __window, Resourcepack* __res, sf::Music* __music);
};


#endif

