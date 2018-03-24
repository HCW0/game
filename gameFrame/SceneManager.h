#pragma once
#include "system.h"
#include "scene.h"
#define MAP std::map<const char*,Scene*>
class Scene;
class SceneManager
{
private:
	Scene* current = nullptr;
	Scene*reserved = nullptr;
	MAP Container;

public:
	void Init();
	void Progress();
	void Render();
	void Kill();
	Scene* GetCurrentScene();
	void SetReservedScene(const char* name);
	void RegistScene(const char* name, Scene* s);
	SceneManager();
	~SceneManager();
};

