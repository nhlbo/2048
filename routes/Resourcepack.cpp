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

std::string Resourcepack::getTexture(const char* key1) {
	return $FOLDER"textures/" + m_data["textures"][key1].get<std::string>();
}

void Resourcepack::setButton(Button& button, const char* key) {
	button.setTexture($FOLDER"textures/" + m_data["textures"][key]["link"].get<std::string>());
	button.setPosition(getPair(m_data["textures"][key]["position"]));
	button.setSize(getPair(m_data["textures"][key]["size"]));
}
