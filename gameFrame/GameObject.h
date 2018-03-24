#pragma once
class GameObject
{
public:
	int x = 0,y = 0;
	void Init();
	void Update();
	void Render();
	void Kill();

	GameObject();
	virtual ~GameObject();
};

