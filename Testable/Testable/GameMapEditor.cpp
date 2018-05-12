#include <SDL.h>

#include "GameMapEditor.h"
#include "GraphicsEngine.h"
#include "GrassTerrainTile.h"
#include "GameTimer.h"
#include "Player.h"

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

GameMapEditor::GameMapEditor(int defaultMapWidth, int defaultMapHeight)
{
	_defaultMapWidth = defaultMapWidth;
	_defaultMapHeight = defaultMapHeight;

	_editedMap = new GameMap(_defaultMapWidth, _defaultMapHeight);
}

void GameMapEditor::loadMap(string mapName)
{

}

void GameMapEditor::saveMap()
{

}

void GameMapEditor::start(GraphicsEngine* graphicsEngine, int screenWidth, int screenHeight)
{
	Player* mapEditor = new Player("000000", screenWidth, screenHeight);

	SDL_Event e;
	bool quit = false;

	GameTimer fpsTimer;
	GameTimer capTimer;
	int countedFrames = 0;
	fpsTimer.start();

	while (!quit)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				
			}

			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_1:
					_selectedBrushId = 1;
					break;

				case SDLK_2:
					_selectedBrushId = 2;
					break;

				case SDLK_3:
					_selectedBrushId = 3;
					break;
				}
			}
		}
		graphicsEngine->refreshScene(_editedMap, mapEditor->getCameraX(), mapEditor->getCameraY());
		int frameTicks = capTimer.getTicks();
		if (frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			// Capped at 20 atm, but VSync is active so can be turned off if necessary
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
		mapEditor->scrollCamera();
	}
}