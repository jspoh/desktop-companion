#pragma once

class Scene {
public:
	virtual ~Scene() {};
	virtual void load() {};
	virtual void init() {};
	virtual void update() {};
	virtual void render() {};
	virtual void cleanup() {};
};