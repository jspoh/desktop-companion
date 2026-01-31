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
	path = "assets/CatItems/Decorations/CatRoomDecorations.png";
	tm.registerTexture(furnitureTextureRef, path);
	tm.createSprite(furnitureRef, furnitureTextureRef, 0, 0, 0, 0, 0, 0, false, 0.f, true);
	furnitureSprite = &tm.getSprite(furnitureRef);
}

void Room::init() {

}

void Room::update(float dt) {
	if (gm.showEditor) {
		sprite->visible = true;
		furnitureSprite->visible = true;
	}
	else {
		sprite->visible = false;
		furnitureSprite->visible = false;
	}

	sprite->sprite.setScale({ localScale * Settings::roomScale, localScale * Settings::roomScale });
	furnitureSprite->sprite.setScale({ localScale * Settings::roomScale, localScale * Settings::roomScale });

	//for (Furniture& f : Settings::furnitures) {
	//	tm.getSprite(f.spriteRef).visible = !f.inInventory;
	//}
}

void Room::addFurniture(Furniture::TYPE type) {
	Furniture f;
	f.type = type;
	f.inInventory = false;
	f.pos = win.getSize() / 2.f;
	f.spriteRef = "Furniture_" + std::to_string(Furniture::count++);

	const Furniture::OffsetData& od = Furniture::spritesheetOffsets.at(type);

	tm.createSprite(f.spriteRef, furnitureTextureRef, 0, 0, od.left, od.top, od.width, od.height, false, 0.f, !f.inInventory);

	Settings::furnitures.push_back(f);

	Settings::save();
}
