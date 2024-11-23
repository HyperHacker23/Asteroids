#pragma once

#include "Sprite.h"
#include "Renderer.h"

#include <iostream>

class Game
{
public:
	Game();
	~Game();

	void runGame();

private:
	const uint16_t SCREEN_WIDTH = 1280;
	const uint16_t SCREEN_HEIGHT = 720;

	Texture2D BackgroundTexture;

	Sprite m_SpaceShip;
	Sprite m_HeavyRock;
	Sprite m_LightRock;

	Camera2D PlayerCam;

	void HandleInputs();
	void ChaseSpaceShip(Sprite& sprite, float VelocityFactor);

	void GameStartup();
	void GameUpdate();
	void GameRender() const;
	void GameShutdown();
};