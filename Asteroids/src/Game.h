#pragma once

#include "Sprite.h"
#include "Renderer.h"

#include <iostream>
#include <vector>

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
	Texture2D BulletTexture;

	Sprite m_SpaceShip;
	Sprite m_HeavyRock;
	Sprite m_LightRock;
	std::vector<Sprite> m_Bullets;

	Camera2D PlayerCam;

	static Rectangle GetViewRect(Camera2D camera, float ScreenWidth, float ScreenHeight);

	void HandleInputs();
	void ChaseSpaceShip(Sprite& sprite, float VelocityFactor);

	void MakeBullets();
	void UpdateBullets();

	void GameStartup();
	void GameUpdate();
	void GameRender() const;
	void GameShutdown();
};