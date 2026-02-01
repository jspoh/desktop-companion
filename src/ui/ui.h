#pragma once


#include "pch.h"
#include "TextureManager.h"
#include "entities/Cat.h"
#include "Room/Room.h"
#include "BreakData.h"
#include "GameManager.h"


class UI {
private:
	inline static const ImVec2 PADDING{ 20, 20 };
	inline static ImVec2 lastWinSize{ PADDING };
	inline static ImVec2 orientation{ 0,0 };

public:
	static void init();

	template<typename fn>
	static void renderImageButtonsTab(const ImVec2& pos, const std::string& title, const std::string& instruction, const std::vector<std::string>& refs, int entriesPerLine, float w, float h, fn callback, const std::vector<bool>& disabled);

	template<typename fn>
	static void renderImageButtonsWindow(const ImVec2& pos, const std::string& title, const std::string& instruction, const std::vector<std::string>& refs, int entriesPerLine, float w, float h, fn callback, const std::vector<bool>& disabled);

	static void _furnitureInventoryTab();

	static void _furniturePurchase();

	static void skinSelect();

	static void _roomSelectTab();

	static void settingsView();

	static void coinsView();

	static void room();

	static void cheatcode();

	static void manageFurniture();

	static void render();
};