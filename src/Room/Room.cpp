#include "pch.h"
#include "Room.h"
#include "GameManager.h"


Room::Room() {

	// load rooms
	std::string path = "assets/CatItems/Rooms";
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_directory()) continue;

		const std::string filename = entry.path().filename().string();
		tm.registerTexture(filename, path + "/" + filename);
		
		// all sprites needed for imgui rendering
		tm.createSprite(filename, filename, 0, 0, 0, 0, width, height, false, 0.1f, false);
		roomRefs.push_back(filename);
	}

	for (const auto& ref : roomRefs) {
		auto& js = tm.getSprite(ref);
		auto& s = js.sprite;

		js.visible = false;
	}

	tm.createSprite(ref, roomRefs.at(0), 0, 0, 0, 0, width, height, false, 0.f, true);
	sprite = &tm.getSprite(ref);
	sprite->sprite.setOrigin(sprite->sprite.getLocalBounds().size / 2.f);
	localScale = win.getSize().x / 2.f < 1.f / width * MAX_WIDTH ? (int)(1.f / width * (win.getSize().x / 2.f)) : 1.f / width * MAX_WIDTH;
	sprite->sprite.setScale({ localScale * Settings::roomScale, localScale * Settings::roomScale });
	sprite->sprite.setPosition(Window::get().getWindow().getSize() / 2.f);

	// load furniture
	path = "assets/CatItems/Decorations";


}

void Room::init() {

}

void Room::update(float dt) {
	if (gm.showEditor) {
		sprite->visible = true;
	}
	else {
		sprite->visible = false;
	}

	sprite->sprite.setScale({ localScale * Settings::roomScale, localScale * Settings::roomScale });
}
