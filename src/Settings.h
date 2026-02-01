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
	inline static bool unlockAll;

	// coins
	inline static int coins;

	// unlocked skins
	inline static std::vector<std::string> unlockedSkins;

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
	inline static bool paused;

	// save user furniture config
	// inline static std::string favouriteBeanBag
	// inline static sf::Vector2f beanBagOffset;		// offset from center of room

	inline static std::filesystem::path configFilePath;

	static void cleanup() {
		unlockedSkins.clear();
		catTexRef.clear();
		roomTexRef.clear();
		furnitures.clear();
		configFilePath.clear();
	}


	static void initConfig() {
		// default values
		unlockAll = false;
		coins = 0;
		unlockedSkins = { "AllCats.png" };
		catTexRef = "AllCats.png";
		roomTexRef = "Room1.png";
		catScale = 1.f;
		roomScale = 1.f;
		catFollowsMouseClick = false;
		catTalks = true;
		furnitures = {};
		catPoops = true;
		paused = false;
	}


	static void init() {
		namespace fs = std::filesystem;
		char* homeDir = nullptr;
		size_t len = 0;
		if (_dupenv_s(&homeDir, &len, "USERPROFILE") != 0 || !homeDir) {
			std::cerr << "Failed to find home dir" << std::endl;
			throw std::runtime_error("Failed to find home dir");
		}
		const fs::path docsPath = fs::path(homeDir) / "Documents";
		free(homeDir);
		static constexpr const char* configDirName = "jspoh desktop companion";
		if (!fs::exists(docsPath / configDirName)) {
			fs::create_directories(docsPath / configDirName);
		}
		static constexpr const char* configFileName = "jdc";
		configFilePath = docsPath / configDirName / configFileName;
		bool configExists = fs::exists(configFilePath);
		if (configExists) {
			std::ifstream ifs(configFilePath);
			json j;
			ifs >> j;

			unlockAll = j.value("unlockAll", false);
			coins = j.value("coins", 0);
			unlockedSkins = j.value("unlockedSkins", std::vector<std::string>{"AllCats.png"});
			catTexRef = j.value("catTexRef", "AllCats.png");
			roomTexRef = j.value("roomTexRef", "Room1.png");
			catScale = j.value("catScale", 1.f);
			roomScale = j.value("roomScale", 1.f);
			catFollowsMouseClick = j.value("catFollowsMouseClick", false);
			catTalks = j.value("catTalks", true);
			furnitures = j.value("furnitures", std::vector<Room::Furniture>{});
			catPoops = j.value("catPoops", true);
			paused = false;
		}
		else {
			initConfig();
			save();
		}

		// init furnitures
		for (const Room::Furniture& f : furnitures) {
			Room::get().addFurniture(f);
		}
	}


	static void save() {
		json j;
		j["unlockAll"] = unlockAll;
		j["coins"] = coins;
		j["unlockedSkins"] = unlockedSkins;
		j["catTexRef"] = catTexRef;
		j["roomTexRef"] = roomTexRef;
		j["catScale"] = catScale;
		j["roomScale"] = roomScale;
		j["catFollowsMouseClick"] = catFollowsMouseClick;
		j["catTalks"] = catTalks;
		j["furnitures"] = furnitures;
		j["catPoops"] = catPoops;

		std::ofstream ofs(configFilePath);
		ofs << j.dump();
	}
};

