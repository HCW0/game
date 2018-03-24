#pragma once
#include "Scene.h"
#include "DoubleBuffer.h"
#include "SceneManager.h"

extern DoubleBuffer* DBUFFER;

class Title :
	public Scene
{
public:
	void Init(SceneManager * target);
	void Update();
	void Render();
	void Kill();
	Title();
	~Title();
};

