#include "..\controls\Resourcepack.h"

bool Resourcepack::setResourcepack(const char* pack_name) {
	m_folder = std::string($RES_FOLDER) + pack_name;
	//load "pack.json" of resourcepack
	std::ifstream inp($FOLDER"pack.json");
	if (inp.is_open()) {
		inp >> m_data;
		inp.close();
		return 1;
	}
	return 0;
}

json& Resourcepack::operator[](const char* key) {
	return m_data[key];
}

std::pair<float, float> Resourcepack::getPair(json& dir) {
	json::iterator it = dir.begin();
	return std::pair<float, float>(*it, *(it + 1));
}

std::string Resourcepack::getFont() {
	return $FOLDER"fonts/" + m_data["fonts"].get<std::string>();
}

std::string Resourcepack::getSound(const char* key, int number) {
	int cnt = 0;
	for (auto& sound_name : m_data["sounds"][key].items()) {
		if (cnt++ == number) {
			//std::cout << "Playing: " << sound_name << "\n";
			return $FOLDER"sounds/" + sound_name.value().get<std::string>();
		}
	}
	number %= cnt;
	for (auto& sound_name : m_data["sounds"][key].items()) {
		if (number-- == 0) {
			//std::cout << "Playing: " << sound_name << "\n";
			return $FOLDER"sounds/" + sound_name.value().get<std::string>();
		}
	}
}

std::string Resourcepack::getNextSound(const char* key) {
	if (m_music.find(std::string(key)) == m_music.end())
		m_music[std::string(key)] = -1;
	int m = ++m_music[std::string(key)];
	return getSound(key, m);
}

std::string Resourcepack::getTexture(const char* key1, const char* key2) {
	if (key2 == nullptr)
		return $FOLDER"textures/" + m_data["textures"][key1].get<std::string>();
	return $FOLDER"textures/" + m_data["textures"][key1][key2].get<std::string>();
}

void Resourcepack::setButton(Button& button, const char* key) {
	button.setTexture($FOLDER"textures/" + m_data["textures"]["buttons"][key]["link"].get<std::string>());
	button.setPosition(getPair(m_data["textures"]["buttons"][key]["position"]));
	button.setSize(getPair(m_data["textures"]["buttons"][key]["size"]));
}

// class Game::skin
void G2048::Skin::init(RenderWindow* __window, Resourcepack* __res, Music* __music) {
	window = __window;
	res = __res;
	music = __music;

	std::ifstream inp($RES_FOLDER"resourcepack.json");
	if (!inp.is_open()) return;
	inp >> m_res;
	inp.close();

	res->setResourcepack((m_res["resourcepack"].get<std::string>()).c_str());
	music->openFromFile(res->getNextSound("music"));

	position = 0;
	menu.setFillColor(Color(0, 0, 0, 200));
	menu.setPosition(Vector2f(MENU_X, MENU_Y));
	//menu.setSize(Vector2f(MENU_WIDTH, 128));
}

void G2048::Skin::open() {
	font.loadFromFile(res->getFont());

	// filesystem C++17: get all file in folder
	m_list.clear();
	for (const auto& entry : std::filesystem::directory_iterator($RES_FOLDER)) {
		std::ifstream pack(entry.path().string() + "/pack.json");		// check is resourcepack or not
		if (!pack.is_open()) continue;
		pack.close();

		m_list.push_back(entry.path().filename().string());
	}
	menu.setSize(Vector2f(MENU_WIDTH, 32 * min<int>(SKINS, m_list.size())));

	// optimize animation
	sf::Texture tex;
	tex.create(950, 720);
	tex.update(*window);
	sf::Sprite background(tex);
	render(background);
	
	while (window->isOpen()) {
		if (music->getStatus() == sf::Sound::Status::Stopped) {
			music->openFromFile(res->getNextSound("music"));
			music->play();
		}
		Event e;
		while (window->pollEvent(e)) {
			if (e.type == Event::Closed || (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)) {
				window->close();
				return;
			}
			if (e.type == Event::MouseWheelMoved) {
				Vector2i coorMouse = Mouse::getPosition(*window);
				if (MENU_X <= coorMouse.x && coorMouse.x <= MENU_X + MENU_WIDTH &&
					MENU_Y <= coorMouse.y && coorMouse.y <= MENU_Y + menu.getLocalBounds().height) {

					if (e.mouseWheel.delta == 1 && position + SKINS < m_list.size()) {
						position++;
						render(background);
					}
					else if (e.mouseWheel.delta == -1 && position > 0) {
						position--;
						render(background);
					}
				}
			}
			if (e.type == Event::MouseButtonReleased) {
				for (int i = position; i < position + SKINS && i < m_list.size(); ++i) {
					Vector2i coorMouse = Mouse::getPosition(*window);
					if (MENU_X <= coorMouse.x && coorMouse.x <= MENU_X + MENU_WIDTH &&
						MENU_Y + 32 * (i - position) <= coorMouse.y && coorMouse.y <= MENU_Y + 32 * (i - position + 1)) {

						m_res["resourcepack"] = m_list[i];
						std::ofstream out($RES_FOLDER"resourcepack.json");
						if (!out.is_open()) return;
						out << m_res;
						out.close();

						res->setResourcepack(m_list[i].c_str());
						music->openFromFile(res->getNextSound("music"));
						return;
					}
				}

				return;
			}
		}
	}
}

void G2048::Skin::draw(string& skinname, int x, int y) {
	Text text;
	text.setFont(font);
	text.setString(skinname);
	text.setCharacterSize(15);
	text.setFillColor(Color::White);
	text.setPosition(x + 16, y + 5);
	window->draw(text);
}

void G2048::Skin::render(const Sprite& background) {
	window->draw(background);
	window->draw(menu);
	for (int i = position; i < position + SKINS && i < m_list.size(); ++i) {
		draw(m_list[i], MENU_X, MENU_Y + 32 * (i - position));
	}
	window->display();
}
