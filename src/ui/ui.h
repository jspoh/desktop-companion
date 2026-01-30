#pragma once


#include "pch.h"
#include "TextureManager.h"
#include "entities/Cat.h"


class UI {
private:


public:
	static void init() {
		
	}

	static void skinSelect() {
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always); // Auto-size

		ImGui::Begin("Companion skin", nullptr,
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoMove);

		ImGui::Text("Select skin");
		ImGui::Separator();

		ImGui::BeginGroup();
		for (const auto& spriteRef : Cat::get().catSpriteRefs) {
			//ImGui::SameLine();
			//ImGui::Image(tm.getSprite(spriteRef).sprite, sf::Color::White, sf::Color::Transparent);
		}
		ImGui::EndGroup();

		ImGui::End();
	}

	static void render() {
		skinSelect();
	}
};
