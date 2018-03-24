#include "Title.h"




void Title::Init(SceneManager * target)
{
	ScnMgr = target;
}

void Title::Update()
{


}

void Title::Render()
{
	DBUFFER->WriteBuffer(0,0,"a");
}


Title::Title()
{
}


Title::~Title()
{
}
