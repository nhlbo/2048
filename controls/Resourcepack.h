#ifndef RESOURCEPACK_H
#define RESOURCEPACK_H
#include <SFML/Audio.hpp>					
#include <SFML/Graphics.hpp>
#include <filesystem>						// C++17: get all file in folder
#include <fstream>	
#include "..\include\nlohmann\json.hpp";	// json: read and write *.json
#include "Button.h"
#include "Cell.h"

#define $RES_FOLDER "./resourcepacks/"
#define $FOLDER m_folder+'/'+

using json = nlohmann::json;

using namespace sf;
using namespace std;

class Resourcepack {
	std::string m_folder;
	std::map <std::string , int> m_music;
	json m_data;

public:
	bool setResourcepack(const char* pack_name);							// dir: ./resourcepacks/<pack_name>
	json& operator[](const char* key);										// use as json[key1][key2]...[keyn]

	std::pair<float, float> getPair(json& dir);								// convert value { [x, y, ...] } to pair<x, y> (type = float)
	std::string getFont();
	std::string getSound(const char* key, int number = 0);					// can read : "sound": <link> and "sound": [<link1>, <link2>, ...]
	std::string getNextSound(const char* key);								// read next: use map to save cur sound of key 
	std::string getTexture(const char* key1, const char* key2 = nullptr);
	
	void setButton(Button& button, const char* key);						// key = name of button										
};


#define SKINS 5
#define MENU_X 750
#define MENU_Y 362
#define MENU_WIDTH 110
namespace G2048 { class Skin; }

class G2048::Skin {
public:
	void init(RenderWindow* __window, Resourcepack* __res, Music* __music);
	void open();

private:
	// shared attribute 
	RenderWindow* window;
	Resourcepack* res;
	Music* music;

	json m_res;
	Font font;
	std::vector<string> m_list;
	int position;
	RectangleShape menu;

	void draw(string& skinname, int x, int y);
	void render(const Sprite& background);
};
#endif