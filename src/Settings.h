#pragma once
#include "pch.h"

// could pack struct to optimize space
struct Settings {
	static bool unlockAll;

	// coins
	static int coins;

	// unlocked skins
	static int unlockedSkinsSize;
	static char unlockedSkins[10][50];

	// save user cat skin and room
	static char catTexRef[50];
	static char roomTexRef[50];

	// save user behaviour preferences
	static float catScale;
	static float roomScale;
	static bool catFollowsMouseClick;
	static bool catTalks;

	// save user furniture config
	// static std::string favouriteBeanBag
	// static sf::Vector2f beanBagOffset;		// offset from center of room

	static std::filesystem::path configFilePath;

	static void initConfig() {
		// default values
		unlockAll = false;
		coins = 0;
		unlockedSkinsSize = 1;
		strcpy_s(unlockedSkins[0], "AllCats.png");
		strcpy_s(catTexRef, "AllCats.png");
		strcpy_s(roomTexRef, "Room1.png");
		catScale = 1.f;
		roomScale = 1.f;
		catFollowsMouseClick = false;
		catTalks = true;
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
		static constexpr const char* configDirName = "jspoh desktop companion";
		if (!fs::exists(docsPath / configDirName)) {
			fs::create_directories(docsPath / configDirName);
		}
		static constexpr const char* configFileName = "jdc";
		configFilePath = docsPath / configDirName / configFileName;

		bool configExists = fs::exists(configFilePath);

		if (configExists) {
			std::ifstream ifs(configFilePath, std::ios::binary);
			if (ifs.is_open()) {
				ifs.read((char*)&unlockAll, sizeof(bool));
				ifs.read((char*)&coins, sizeof(int));
				ifs.read((char*)&unlockedSkinsSize, sizeof(int));
				ifs.read((char*)&unlockedSkins, sizeof(unlockedSkins));
				ifs.read((char*)&catTexRef, sizeof(catTexRef));
				ifs.read((char*)&roomTexRef, sizeof(roomTexRef));
				ifs.read((char*)&catScale, sizeof(float));
				ifs.read((char*)&roomScale, sizeof(float));
				ifs.read((char*)&catFollowsMouseClick, sizeof(bool));
				ifs.read((char*)&catTalks, sizeof(bool));
			}
			else {
				configExists = false;
			}
		}

		if (!configExists) {
			initConfig();
			save();
		}
	}

	static void save() {
		std::ofstream ofs(configFilePath, std::ios::binary);
		ofs.write((char*)&unlockAll, sizeof(unlockAll));
		ofs.write((char*)&coins, sizeof(coins));
		ofs.write((char*)&unlockedSkinsSize, sizeof(unlockedSkinsSize));
		ofs.write((char*)&unlockedSkins, sizeof(unlockedSkins));
		ofs.write((char*)&catTexRef, sizeof(catTexRef));
		ofs.write((char*)&roomTexRef, sizeof(roomTexRef));
		ofs.write((char*)&catScale, sizeof(catScale));
		ofs.write((char*)&roomScale, sizeof(roomScale));
		ofs.write((char*)&catFollowsMouseClick, sizeof(catFollowsMouseClick));
		ofs.write((char*)&catTalks, sizeof(catTalks));
	}
};


inline bool Settings::unlockAll;
inline int Settings::coins;
inline int Settings::unlockedSkinsSize;
inline char Settings::unlockedSkins[10][50];
inline char Settings::catTexRef[50];
inline char Settings::roomTexRef[50];
inline float Settings::catScale;
inline float Settings::roomScale;
inline bool Settings::catFollowsMouseClick;
inline bool Settings::catTalks;
inline std::filesystem::path Settings::configFilePath;
