#include "GraphicsEngine.h"
#include "Sprite.h"
#include "Unit.h"
#include "GameAssetManager.h"
#include "GameMap.h"
#include "Player.h"
#include "Terrain.h"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <map>
#include <stdexcept>
#include <memory>

using namespace std;

GraphicsEngine::GraphicsEngine(sdl2::RendererSharedPtr gameRenderer, std::shared_ptr<GameAssetManager> assetManager, unsigned int resolutionX, unsigned int resolutionY)
{
	_gameAssetManager = assetManager;
	_gameRenderer = gameRenderer;
	_windowResolutionX = resolutionX;
	_windowResolutionY = resolutionY;

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) 
	{
		throw std::runtime_error("Could not initialize PNG filetype loading");
	}
}

GraphicsEngine::~GraphicsEngine()
{
	IMG_Quit();
}

void GraphicsEngine::_setCamera(unsigned int cameraX, unsigned int cameraY)
{
	_currentPlayerCameraX = cameraX;
	_currentPlayerCameraY = cameraY;
}

void GraphicsEngine::refreshScene(std::shared_ptr<GameMap> gameMap, unsigned int cameraX, unsigned int cameraY)
{
	_setCamera(cameraX, cameraY);
	SDL_SetRenderDrawColor(_gameRenderer.get(), 0, 0, 0, 250);
	SDL_RenderClear(_gameRenderer.get());

	_drawGameMap(gameMap);
	
	SDL_RenderPresent(_gameRenderer.get());
}

void GraphicsEngine::refreshScene(std::vector<std::shared_ptr<Unit>> units, std::shared_ptr<GameMap> gameMap, unsigned int cameraX, unsigned int cameraY)
{
	_setCamera(cameraX, cameraY);
	SDL_SetRenderDrawColor(_gameRenderer.get(), 0, 0, 0, 250);
	SDL_SetRenderTarget(_gameRenderer.get(), 0);
	SDL_RenderClear(_gameRenderer.get());

	_drawGameMap(gameMap);
	_drawUnits(units);

	SDL_RenderPresent(_gameRenderer.get());
}

void GraphicsEngine::_drawUnits(std::vector<std::shared_ptr<Unit>> units)
{
	for (auto const& unit : units)
	{
		bool unitAlive = unit -> isAlive();
		if (unitAlive)
		{
			_drawGameObject(unit.get());
		}
	}
}

void GraphicsEngine::_drawGameMap(std::shared_ptr<GameMap> gameMap)
{
	std::vector<std::shared_ptr<Terrain>> mapTerrain = gameMap -> getTerrain();
	for (auto const& terrain : mapTerrain)
	{
		_drawGameObject(terrain.get());
	}
}

void GraphicsEngine::_drawGameObject(GameObject* object)
{
	unsigned int spriteId = object->getSpriteId();
	unsigned int objectPositionX = object->getPositionX();
	unsigned int objectPositionY = object->getPositionY();
	unsigned int objectWidth = object->getWidth();
	unsigned int objectHeight = object->getHeight();

	std::shared_ptr<Sprite> objectSprite = _gameAssetManager->getSprite(spriteId);
	SDL_Rect spriteClip = objectSprite->getClip(1);

	if (_isInCamera(objectPositionX, objectPositionY, objectWidth, objectHeight))
	{
		_drawTexture(objectSprite->getTexture(), spriteClip, objectPositionX - _currentPlayerCameraX, objectPositionY - _currentPlayerCameraY);
	}
}

bool GraphicsEngine::_isInCamera(unsigned int positionX, unsigned int positionY, unsigned int width, unsigned int height)
{
	if (positionX + width < _currentPlayerCameraX)
	{
		return false;
	}

	if (positionX > _currentPlayerCameraX + _windowResolutionX)
	{
		return false;
	}

	if (positionY > _currentPlayerCameraY + _windowResolutionY)
	{
		return false;
	}

	if (positionY + height < _currentPlayerCameraY)
	{
		return false;
	}

	return true;
}

void GraphicsEngine::_drawTexture(sdl2::TextureSharedPtr texture, SDL_Rect clip, unsigned int positionX, unsigned int positionY)
{
	SDL_Rect unitPosition;
	unitPosition.x = positionX;
	unitPosition.y = positionY;
	unitPosition.w = clip.w;
	unitPosition.h = clip.h;

	SDL_RenderCopy(_gameRenderer.get(), texture.get(), &clip, &unitPosition);
}