#pragma once


#include <memory>
#include "Scene.h"


class SceneManager {
private:
	SceneManager() {};
	~SceneManager() {};

	std::shared_ptr<Scene> curr = nullptr;
	std::shared_ptr<Scene> next = nullptr;

public:
	static SceneManager& get() {
		static SceneManager sm;
		return sm;
	}

	void setScene(const std::shared_ptr<Scene>& scn) {
		next = scn;
	}

	void update() {
		if (next != nullptr) {
			if (curr != nullptr) curr->cleanup();
			next->load();
			next->init();
			curr = next;
			next = nullptr;
		}

		curr->update();
		curr->render();
	}
};
