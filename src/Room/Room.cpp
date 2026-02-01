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
	//tm.createSprite(furnitureRef, furnitureTextureRef, 0, 0, 0, 0, 0, 0, false, 0.f, true);
	//furnitureSprite = &tm.getSprite(furnitureRef);
}

Room::Furniture::Furniture() {

}

Room::Furniture::~Furniture() {

}

void Room::init() {

}

void Room::update(float dt) {
	sf::Vector2f mPos = sf::Mouse::getPosition() * 1.f;

	if (gm.showEditor) {
		sprite->visible = true;
	}
	else {
		sprite->visible = false;
	}

	sprite->sprite.setScale({ localScale * Settings::roomScale, localScale * Settings::roomScale });

	for (Furniture& f : Settings::furnitures) {
		// update AABB
		const Furniture::OffsetData& od = Furniture::spritesheetOffsets.at(f.type);
		f.AABB_MIN = { f.pos.x - od.width / 2.f, f.pos.y - od.height / 2.f };
		f.AABB_MAX = { f.pos.x + od.width / 2.f, f.pos.y + od.height / 2.f };

		TextureManager::JS_SPRITE& furnitureSprite = tm.getSprite(f.spriteRef);

		// update sprite pos
		furnitureSprite.sprite.setPosition(f.pos);

		// update visibility
		if (gm.showEditor) {
			furnitureSprite.visible = true;
		}
		else {
			furnitureSprite.visible = false;
		}

		// update scale
		furnitureSprite.sprite.setScale({ localScale * Settings::roomScale * (f.mirrored ? -1.f : 1.f), localScale * Settings::roomScale});
	}

#pragma region drag_furniture
	// drag furniture to reposition them
	static int draggedFurnitureIdx = -1;		// is ok, because is singleton

	static bool isDragging = false;
	static bool prevIsDragging = false;
	// check before running loop. cheaper this way
	if (draggedFurnitureIdx == -1 && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && Cat::get().getEntityState() != Cat::EntityStates::DRAGGED) {
		for (int i{}; i < (int)Settings::furnitures.size(); ++i) {
			//tm.getSprite(f.spriteRef).visible = !f.inInventory;

			if (Settings::furnitures[i].inInventory) continue;		// check before allocating memory for temp var. will be cheaper
			Furniture& f = Settings::furnitures[i];


			if (mPos.x >= f.AABB_MIN.x && mPos.x <= f.AABB_MAX.x && mPos.y >= f.AABB_MIN.y && mPos.y <= f.AABB_MAX.y) {
				draggedFurnitureIdx = i;
				isDragging = true;
				selectedFurnitureIdx = i;
				break;			// only drag one at atime
			}
		}
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		draggedFurnitureIdx = -1;		// reset drag furniture state once user releases
		isDragging = false;
	}

	if (!isDragging && prevIsDragging) {
		Settings::save();
	}

	prevIsDragging = isDragging;

	if (isDragging && draggedFurnitureIdx != -1) {
		Settings::furnitures[draggedFurnitureIdx].pos = sf::Mouse::getPosition() * 1.f;
	}

#pragma endregion drag_furniture
}

void Room::addFurniture(Furniture::TYPE type) {
	Furniture f;
	f.type = type;
	f.inInventory = false;
	f.pos = win.getSize() / 2.f;
	f.spriteRef = "Furniture_" + std::to_string(Furniture::count++);

	const Furniture::OffsetData& od = Furniture::spritesheetOffsets.at(type);

	f.AABB_MIN = { f.pos.x - od.width / 2.f, f.pos.y - od.height / 2.f };
	f.AABB_MAX = { f.pos.x + od.width / 2.f, f.pos.y + od.height / 2.f };

	tm.createSprite(f.spriteRef, furnitureTextureRef, 0, 0, od.left, od.top, od.width, od.height, false, 0.f, !f.inInventory);

	TextureManager::JS_SPRITE& s = tm.getSprite(f.spriteRef);
	s.sprite.setOrigin(s.sprite.getLocalBounds().size / 2.f);

	Settings::furnitures.push_back(f);

	//Settings::save();
}


void Room::addFurniture(const Furniture& f) {
	const Furniture::OffsetData& od = Furniture::spritesheetOffsets.at(f.type);
	tm.createSprite(f.spriteRef, furnitureTextureRef, 0, 0, od.left, od.top, od.width, od.height, false, 0.f, !f.inInventory);

	TextureManager::JS_SPRITE& s = tm.getSprite(f.spriteRef);
	s.sprite.setOrigin(s.sprite.getLocalBounds().size / 2.f);
}
