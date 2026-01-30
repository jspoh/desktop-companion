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
		ImGui::Begin("Companion skin");

		ImGui::Text("Select skin");
		ImGui::Separator();

		ImGui::BeginGroup();
		for (const auto& texRef : Cat::get().catTextures) {
			ImGui::SameLine();
			ImGui::Image(tm.getSprite(texRef).sprite, sf::Color::White, sf::Color::Transparent);
		}
		ImGui::EndGroup();

		ImGui::End();
	}

	static void render() {
		skinSelect();
	}
};
