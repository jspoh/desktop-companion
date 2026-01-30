#include "pch.h"
#include "Room.h"
#include "TextureManager.h"


Room::Room() {

	// load rooms
	std::string path = "assets/CatItems/Rooms";
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_directory()) continue;

		const std::string filename = entry.path().filename().string();
		tm.registerTexture(filename, path + filename);
		tm.createSprite(filename, filename, 0, 0, 0, 0, 0, 0, false, 0.1f, false);
		roomRefs.push_back(filename);
	}

	// load furniture
	path = "assets/CatItems/Decorations";


}
