#pragma once

#include "pch.h"
#include "Window.h"


class TextureManager {
public:
	struct JS_SPRITE {
		JS_SPRITE(const sf::Sprite& s) : sprite(s) {}
		JS_SPRITE(
			const sf::Sprite& s,
			int numFrames,
			int xoffset,
			int left,
			int top,
			int width,
			int height,
			bool playingAnimation,
			float animationAdvanceTime = 0.1f,
			int animationLoopCount = -1,
			bool visible = true
		)
			: numFrames{ numFrames },
			xoffset{ xoffset },
			sprite(s),
			left(left),
			top(top),
			width(width),
			height(height),
			playingAnimation{ playingAnimation },
			animationAdvanceTime{ animationAdvanceTime },
			animationLoopCount{ animationLoopCount },
			visible{ visible }
		{
			sprite.setTextureRect(sf::IntRect(
				sf::Vector2i(left + frame * (width + xoffset), top),  // position
				sf::Vector2i(width, height)                // size
			));
		}

		sf::Sprite sprite;

		bool visible{ true };

		int frame{};
		int numFrames{};
		float animationElapsed{};
		bool playingAnimation{ false };
		int animationLoopCount{ -1 };	// -1 = infinite loop
		int animationElapsedLoops{};
		float animationAdvanceTime{};
		int xoffset{};
		int staticFrameIdx{};		// for when no animation or animation ends, which frame to display
		//int yoffset{};
		int z = 0;					// OpenGL like. looking down neg z axis.

		int left{};
		int top{};
		int width{};
		int height{};
	};

private:
	sf::Font font;

	TextureManager() {
		font = sf::Font("assets/Consolas-Regular.ttf");
	}

	~TextureManager() = default;

	std::unordered_map<std::string, sf::Texture> textures;		// loaded into gpu vram
	std::unordered_map<std::string, JS_SPRITE> sprites;
	std::vector<std::reference_wrapper<JS_SPRITE>> spritesValues;						// for fast access of sprites.values()
	std::unordered_map<std::string, sf::Text> texts;

public:
	static TextureManager& get() {
		static TextureManager instance;
		return instance;
	}

	void cleanup();

	void render(float dt);

	bool registerTexture(const std::string& ref, const std::string& path);

	// @param left, top, width, height for sf::IntRect() to set texture rect forr cutting spritesheets
	bool createSprite(const std::string& ref, const std::string& texRef, int num_frames = 0, int xoffset = 0, int left = 0, int top = 0, int width = 0, int height = 0, bool playingAnimation = false, float animationAdvanceTime = 0.1f, bool visible = true);

	sf::Texture& getTexture(const std::string& ref);

	const sf::Texture& getTexture(const std::string& ref) const;

	JS_SPRITE& getSprite(const std::string& ref);

	const JS_SPRITE& getSprite(const std::string& ref) const;

	const std::vector<std::reference_wrapper<JS_SPRITE>>& getAllSprites() const;

	sf::Text& registerText(const std::string& ref, const std::string& content, int fontSize);

	void setTextContent(const std::string& ref, const std::string& content);

	// sort sprites by z value
	void sortSprites();
};
