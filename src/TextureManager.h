#pragma once

#include "defines.h"
#include "Window.h"


class TextureManager {
private:
	sf::Font font;

	TextureManager() {
		font = sf::Font("assets/arial.ttf");
	}

	~TextureManager() = default;

	std::unordered_map<std::string, sf::Texture> textures;		// loaded into gpu vram
	std::unordered_map<std::string, sf::Sprite> sprites;
	std::vector<std::reference_wrapper<sf::Sprite>> spritesValues;						// for fast access of sprites.values()
	std::unordered_map<std::string, sf::Text> texts;

public:
	static TextureManager& get() {
		static TextureManager instance;
		return instance;
	}

	void cleanup() {
		textures.clear();
		sprites.clear();
		spritesValues.clear();
		texts.clear();
	}

	void render() {
		for (const auto& sprite : spritesValues) {
			win.draw(sprite);
		}

		for (const auto& [ref, txt] : texts) {
			win.draw(txt);
		}
	}

	bool registerTexture(const std::string& ref, const std::string& path) {
		try {
			textures.emplace(ref, sf::Texture(path));
		}
		catch (const std::exception& e) {
			std::cerr << "Error loading texture '" << ref << "' from path '"
				<< path << "': " << e.what() << std::endl;
			return false;
		}
		catch (...) {
			return false;
		}
		return true;
	}

	bool createSprite(const std::string& ref, const std::string& texRef) {
		if (textures.find(texRef) == textures.end()) {
			std::cerr << "createSprite > texRef " << texRef << " does not exist" << std::endl;
			return false;
		}
		sprites.emplace(ref, sf::Sprite(textures.at(texRef)));
		spritesValues.push_back(sprites.at(ref));
		return true;
	}

	sf::Texture& getTexture(const std::string& ref) {
		return textures.at(ref);
	}

	const sf::Texture& getTexture(const std::string& ref) const {
		return textures.at(ref);
	}

	sf::Sprite& getSprite(const std::string& ref) {
		return sprites.at(ref);
	}

	const sf::Sprite& getSprite(const std::string& ref) const {
		return sprites.at(ref);
	}

	const std::vector<std::reference_wrapper<sf::Sprite>>& getAllSprites() const {
		return spritesValues;
	}

	sf::Text& registerText(const std::string& ref, const std::string& content, int fontSize) {
		if (texts.find(ref) != texts.end()) {
			std::cerr << "TextureManager > drawText > ref " << ref << " is already in use" << std::endl;
		}

		// Create a Text object with the font first, then emplace it
		auto [it, inserted] = texts.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(ref),
			std::forward_as_tuple(font)  // Construct sf::Text with font reference
		);

		// Now set the string and character size
		it->second.setString(content);
		it->second.setCharacterSize(fontSize);

		return it->second;
	}
};
