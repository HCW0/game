#include "system.h"
#include "Title.h"
#include "DoubleBuffer.h"

const bool GAME_LIFESPAN = true;
SceneManager* SCENE_MANAGER = nullptr;
DoubleBuffer* DBUFFER = nullptr;

void SetWindowCenter() {
	HWND wh = GetConsoleWindow();
	int dwWidth = GetSystemMetrics(SM_CXSCREEN);
	int dwHeight = GetSystemMetrics(SM_CYSCREEN);
	dwWidth = (0.5f*abs(CONSOLE_WIDTH - dwWidth));
	dwHeight = (0.5f* abs(CONSOLE_HEIGHT - dwHeight));
	MoveWindow(wh, dwWidth ,dwHeight, CONSOLE_WIDTH, CONSOLE_HEIGHT, TRUE);
}

void Init(){
	SetConsoleTitle("Advanced C tutorial - programming-technique.blogspot.com");
	SetWindowCenter();

	SCENE_MANAGER = new SceneManager();
	SCENE_MANAGER->Init();
	SCENE_MANAGER->RegistScene("Menu", new Title());
	SCENE_MANAGER->SetReservedScene("Menu");

	DBUFFER = new DoubleBuffer();
	DBUFFER->Init();
}


int main(int argc, char* argv[]) {



	Init();
	while (GAME_LIFESPAN) {
		DBUFFER->ClearBuffer();
		SCENE_MANAGER->Progress();
		SCENE_MANAGER->Render();
		DBUFFER->ChangeBuffer();
	}

	delete  SCENE_MANAGER,DBUFFER;
	return 0;
}