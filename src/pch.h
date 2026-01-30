#pragma once

#ifndef __PCH_H__
#define __PCH_H__

#include <chrono>
#include <memory>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <exception>
#include <functional>
#include <optional>
#include <limits>
#include <map>

#include "imgui.h"
#include "imgui-SFML.h"


#define tm TextureManager::get()
#define gm GameManager::get()
#define sm SceneManager::get()
#define win Window::get().getWindow()


inline sf::Vector2f operator/(const sf::Vector2f& v, float divisor) {
	return { v.x / divisor, v.y / divisor };
}

inline sf::Vector2f operator/(const sf::Vector2u& v, float divisor) {
	return sf::Vector2f{ v } / divisor;
}

inline sf::Vector2f operator*(const sf::Vector2i& v, float mul) {
	return { v.x * mul, v.y * mul };
}


#endif
