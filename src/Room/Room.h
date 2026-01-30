#pragma once

#include "pch.h"


class Room {
private:
	Room();

	std::vector<std::string> roomRefs;
	std::vector<std::string> furnitureRefs;

public:
	static Room& get() {
		static Room instance;
		return instance;
	}
};
