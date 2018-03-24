#pragma once
class SceneManager;
class Scene
{
private :
public:
	SceneManager *ScnMgr = nullptr;
	virtual void Init(SceneManager* target) = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
};

