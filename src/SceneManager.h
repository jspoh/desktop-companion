#pragma once


#include "defines.h"
#include "Scene.h"


class SceneManager {
private:
	SceneManager() {};
	~SceneManager() {};

	std::shared_ptr<Scene> curr = nullptr;
	std::shared_ptr<Scene> next = nullptr;

public:
	static SceneManager& get() {
		static SceneManager instance;
		return instance;
	}

	void setScene(const std::shared_ptr<Scene>& scn) {
		next = scn;
	}

	void update() {

		if (next) {
			if (curr) curr->cleanup();
			next->load();
			next->init();
			curr = next;
			next = nullptr;
		}

		if (!curr) return;
		curr->update();
		curr->render();
	}

	void cleanup() {
		if (curr) curr->cleanup();
	}
};
