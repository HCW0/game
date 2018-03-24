#include "SceneManager.h"



void SceneManager::Init()
{
	
}

void SceneManager::Progress()
{
	if (reserved != nullptr) {
		current = reserved;
		current->Init(this);
		reserved = nullptr;
	}
	current->Update();

}

void SceneManager::SetReservedScene(const char* name)
{
	reserved = Container[name];
}

void SceneManager::Render()
{
	current->Render();
}

void SceneManager::Kill()
{
	delete this;
}

Scene * SceneManager::GetCurrentScene()
{
	return this->current;
}

void SceneManager::RegistScene(const char * name, Scene * s)
{
	if (name == nullptr || s == nullptr) return;
	Container.insert(MAP::value_type(name, s));
}

SceneManager::SceneManager(){}
SceneManager::~SceneManager(){
	for (MAP::iterator it = Container.begin(); it != Container.end(); ++it) {
		delete it->second;
	}
	Container.clear();
}


