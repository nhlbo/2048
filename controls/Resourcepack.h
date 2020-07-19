#ifndef RESOURCEPACK_H
#define RESOURCEPACK_H
#include "..\include\nlohmann\json.hpp";
#include <fstream>
#include "Button.h"
#include "Cell.h"

#define $RES_FOLDER "./resourcepacks/"
#define $FOLDER m_folder+'/'+

using json = nlohmann::json;

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
	std::string getTexture(const char* key1);
	
	
	void setButton(Button& button, const char* key);						// key = name of button										
};
#endif

