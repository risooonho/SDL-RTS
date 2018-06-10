#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <memory>
#include <string>

#include <SDL.h>

#include "Unit.h"
#include "GameMap.h"

class Player
{
private:
	void _handleScrolling(int mousePositionX, int mousePositionY);
	void _resetScrolling();
	std::unique_ptr<std::string> _color;
	std::unique_ptr<std::vector<std::unique_ptr<Unit>>> _units;
	std::vector<std::unique_ptr<Player>> _hostilePlayers;
	std::unique_ptr<GameMap> _gameMap;

	int _cameraX;
	int _cameraY;
	int _screenResolutionX;
	int _screenResolutionY;
	int _mousePositionX;
	int _mousePositionY;
	bool _scrollingLeft;
	bool _scrollingRight;
	bool _scrollingUp;
	bool _scrollingDown;
public:
	Player(std::unique_ptr<string> color, std::vector<std::unique_ptr<Unit>> startingUnits, int screenResolutionX, int screenResolutionY);
	Player(std::unique_ptr<string> color, int screenResolutionX, int screenResolutionY);
	void addUnit(std::unique_ptr<Unit> unit);
	std::vector<std::unique_ptr<Unit>> getUnits();

	void setCamera(int cameraX, int cameraY);
	int getCameraX();
	int getCameraY();
	void handleInteraction(std::unique_ptr<SDL_Event> e, std::vector<std::unique_ptr<Unit>> units);
	void scrollCamera();

	void startNewGame(std::unique_ptr<GameMap> gameMap);
	void setPlayerHostile(std::unique_ptr<Player> player);
};

#endif