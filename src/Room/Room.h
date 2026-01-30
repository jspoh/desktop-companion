#pragma once

#include "pch.h"
#include "TextureManager.h"


class Room {
public:
	static constexpr int width = 512;
	static constexpr int height = width;

private:
	Room();

	std::vector<std::string> roomRefs;
	std::vector<std::string> furnitureRefs;

public:
	static Room& get() {
		static Room instance;
		return instance;
	}

	static constexpr const char* ref = "room";
	TextureManager::JS_SPRITE* sprite = nullptr;

	void init();

	void update(float dt);

	auto& getRoomRefs() const {
		return roomRefs;
	}

	auto& getFurnitureRefs() const {
		return furnitureRefs;
	}
};
