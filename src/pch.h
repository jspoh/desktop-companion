#pragma once

// !TODO: turn into pch

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


#define tm TextureManager::get()
#define gm GameManager::get()
#define sm SceneManager::get()
#define win Window::get().getWindow()
