#pragma once
#include "pch.h"
#include "Room/Room.h"
#include "json.hpp"

using json = nlohmann::json;

namespace nlohmann {
	template <>
	struct adl_serializer<Room::Furniture> {
		static void to_json(json& j, const Room::Furniture& f) {
			j = json{
				{"type", static_cast<int>(f.type)},
				{"inInventory", f.inInventory},
				{"pos", {f.pos.x, f.pos.y}},
				{"spriteRef", f.spriteRef},
				{"AABB_MIN", {f.AABB_MIN.x, f.AABB_MIN.y}},
				{"AABB_MAX", {f.AABB_MAX.x, f.AABB_MAX.y}},
				{"mirrored", f.mirrored},
				{"z", f.z}
			};
		}

		static void from_json(const json& j, Room::Furniture& f) {
			f.type = static_cast<Room::Furniture::TYPE>(j.at("type").get<int>());
			f.inInventory = j.at("inInventory").get<bool>();
			f.pos.x = j.at("pos")[0].get<float>();
			f.pos.y = j.at("pos")[1].get<float>();
			f.spriteRef = j.at("spriteRef").get<std::string>();
			f.AABB_MIN.x = j.at("AABB_MIN")[0].get<float>();
			f.AABB_MIN.y = j.at("AABB_MIN")[1].get<float>();
			f.AABB_MAX.x = j.at("AABB_MAX")[0].get<float>();
			f.AABB_MAX.y = j.at("AABB_MAX")[1].get<float>();
			f.mirrored = j.at("mirrored").get<bool>();
			f.z = j.at("z").get<int>();
		}
	};
}

// could pack struct to optimize space
struct Settings {
	// not to be saved

	inline static bool paused = false;
	inline static bool onEnforcedBreak = false;

	inline static std::vector<bool> lockedSkins;		// for fast access and reduce computation for unlockedSkins ImGui disabling in ui.cpp

	// save
	inline static int MAX_WORK_DURATION_M = 60.f;
	inline static int MIN_BREAK_DURATION_M = 5.f;
	inline static bool enforceBreaks{ false };

	inline static bool unlockAll;

	// coins
	inline static int coins;

	inline static bool gf = false;

	// unlocked skins
	inline static std::unordered_set<std::string> unlockedSkins;

	// save user cat skin and room
	inline static std::string catTexRef;
	inline static std::string roomTexRef;

	// furnitures
	inline static std::vector<Room::Furniture> furnitures;

	// save user behaviour preferences
	inline static float catScale;
	inline static float roomScale;
	inline static bool catFollowsMouseClick;
	inline static bool catTalks;

	inline static bool catPoops;

	// save user furniture config
	// inline static std::string favouriteBeanBag
	// inline static sf::Vector2f beanBagOffset;		// offset from center of room

	inline static std::filesystem::path configFilePath;

	static void cleanup();


	static void initConfig();


	static void init();


	static void _resetLockedSkins();


	static void save();
};

