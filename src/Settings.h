#pragma once
#include "pch.h"
#include "Room/Room.h"


// could pack struct to optimize space
struct Settings {
	static bool unlockAll;

	// coins
	static int coins;

	// unlocked skins
	static std::vector<std::string> unlockedSkins;

	// save user cat skin and room
	static std::string catTexRef;
	static std::string roomTexRef;

	// furnitures
	static std::vector<Room::Furniture> furnitures;

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
		unlockedSkins = { "AllCats.png" };
		catTexRef = "AllCats.png";
		roomTexRef = "Room1.png";
		catScale = 1.f;
		roomScale = 1.f;
		catFollowsMouseClick = false;
		catTalks = true;
		furnitures = {};
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
			std::ifstream ifs(configFilePath, std::ios::binary);
			if (ifs.is_open()) {
				ifs.read((char*)&unlockAll, sizeof(bool));
				ifs.read((char*)&coins, sizeof(int));
				size_t vecSize;
				ifs.read((char*)&vecSize, sizeof(size_t));
				unlockedSkins.resize(vecSize);
				for (size_t i = 0; i < vecSize; ++i) {
					size_t strLen;
					ifs.read((char*)&strLen, sizeof(size_t));
					unlockedSkins[i].resize(strLen);
					ifs.read(&unlockedSkins[i][0], strLen);
				}
				size_t strLen;
				ifs.read((char*)&strLen, sizeof(size_t));
				catTexRef.resize(strLen);
				ifs.read(&catTexRef[0], strLen);
				ifs.read((char*)&strLen, sizeof(size_t));
				roomTexRef.resize(strLen);
				ifs.read(&roomTexRef[0], strLen);
				ifs.read((char*)&catScale, sizeof(float));
				ifs.read((char*)&roomScale, sizeof(float));
				ifs.read((char*)&catFollowsMouseClick, sizeof(bool));
				ifs.read((char*)&catTalks, sizeof(bool));
				ifs.read((char*)&vecSize, sizeof(size_t));
				furnitures.resize(vecSize);
				for (size_t i = 0; i < vecSize; ++i) {
					ifs.read((char*)&furnitures[i], sizeof(Room::Furniture));
				}
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
		size_t vecSize = unlockedSkins.size();
		ofs.write((char*)&vecSize, sizeof(size_t));
		for (const auto& str : unlockedSkins) {
			size_t strLen = str.size();
			ofs.write((char*)&strLen, sizeof(size_t));
			ofs.write(str.data(), strLen);
		}
		size_t strLen = catTexRef.size();
		ofs.write((char*)&strLen, sizeof(size_t));
		ofs.write(catTexRef.data(), strLen);
		strLen = roomTexRef.size();
		ofs.write((char*)&strLen, sizeof(size_t));
		ofs.write(roomTexRef.data(), strLen);
		ofs.write((char*)&catScale, sizeof(catScale));
		ofs.write((char*)&roomScale, sizeof(roomScale));
		ofs.write((char*)&catFollowsMouseClick, sizeof(catFollowsMouseClick));
		ofs.write((char*)&catTalks, sizeof(catTalks));
		vecSize = furnitures.size();
		ofs.write((char*)&vecSize, sizeof(size_t));
		for (const auto& furniture : furnitures) {
			ofs.write((char*)&furniture, sizeof(Room::Furniture));
		}
	}
};


inline bool Settings::unlockAll;
inline int Settings::coins;
inline std::vector<std::string> Settings::unlockedSkins;
inline std::string Settings::catTexRef;
inline std::string Settings::roomTexRef;
inline float Settings::catScale;
inline float Settings::roomScale;
inline bool Settings::catFollowsMouseClick;
inline bool Settings::catTalks;
inline std::filesystem::path Settings::configFilePath;
inline std::vector<Room::Furniture> Settings::furnitures;