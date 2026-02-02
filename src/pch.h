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
#include <filesystem>
#include <fstream>
#include <cstring>
#include <sstream>
#include <deque>
#include <queue>
#include <ctime>
#include <algorithm>

#include "imgui.h"
#include "imgui-SFML.h"

#include "Settings/Settings.h"


#define texM TextureManager::get()
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

inline ImVec2& operator+=(ImVec2& lhs, const ImVec2& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

#define M_PI 3.14159265358979323846f

inline float toRadians(float degrees) {
	return degrees * (M_PI / 180.0f);
}

inline float toDegrees(float radians) {
	return radians * (180.0f / M_PI);
}

inline float lerp(float a, float b, float t) {
	return a + (b - a) * t;
}


#endif
