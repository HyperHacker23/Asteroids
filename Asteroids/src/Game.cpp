#include "Game.h"

#include <raylib.h>
#include <raymath.h>

Game::Game()
	:BackgroundTexture{ NULL },
	m_SpaceShip(100),
	m_HeavyRock(200),
	m_LightRock(75),
	PlayerCam{ {0.0f, 0.0f}, {0.0f,0.0f}, 0.0f, 0.0f }
{

}

Game::~Game()
{

}

void Game::GameStartup()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Sneaky Fox");
	SetTargetFPS(140);

	BackgroundTexture = LoadTexture("res/background.png");

	m_SpaceShip.setRect(0.0f, 0.0f, 98.0f, 75.0f);
	m_HeavyRock.setRect(0.0f, 0.0f, 120.0f, 98.0f);
	m_LightRock.setRect(0.0f, 0.0f, 45.0f, 40.0f);

	m_SpaceShip.setTexture("res/player.png");
	m_HeavyRock.setTexture("res/heavy.png");
	m_LightRock.setTexture("res/light.png");

	m_SpaceShip.setPos({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f });
	m_HeavyRock.setPos({ 200.0f, 200.0f });
	m_LightRock.setPos({ 400.0f, 100.0f });
}

void Game::HandleInputs()
{
	float deltaTime;
	deltaTime = GetFrameTime();

	constexpr float ROTATION_SPEED = 5.0f;

	Vector2 newPos = m_SpaceShip.getPos();

	float currentRotation = m_SpaceShip.getRotation();
	float velocity = m_SpaceShip.getVelocity();

	if (IsKeyDown(KEY_W))
	{
		float radians = DEG2RAD * currentRotation;
		newPos.x += sinf(radians) * velocity * deltaTime;
		newPos.y -= cosf(radians) * velocity * deltaTime;
	}
	if (IsKeyDown(KEY_S))
	{
		float radians = DEG2RAD * currentRotation;
		newPos.x -= sinf(radians) * velocity * deltaTime;
		newPos.y += cosf(radians) * velocity * deltaTime;
	}
	if (IsKeyDown(KEY_A))
	{
		m_SpaceShip.setRotation(currentRotation - ROTATION_SPEED * (velocity / (2 * PI)) * deltaTime);
	}
	if (IsKeyDown(KEY_D))
	{
		m_SpaceShip.setRotation(currentRotation + ROTATION_SPEED * (velocity / (2 * PI)) * deltaTime);
	}

	m_SpaceShip.setPos(newPos);
}

void Game::ChaseSpaceShip(Sprite& sprite, float VelocityFactor)
{
	float deltaTime;
	deltaTime = GetFrameTime();
	Vector2 Direction = m_SpaceShip.getPos() - sprite.getPos();
	float Velocity = m_SpaceShip.getVelocity();

	if (Vector2Length(Direction) > 0)
	{
		Direction = Vector2Normalize(Direction);
	}

	Vector2 newPos = sprite.getPos();
	newPos.x += Direction.x * Velocity * VelocityFactor * deltaTime;
	newPos.y += Direction.y * Velocity * VelocityFactor * deltaTime;

	sprite.setPos(newPos);
}

void Game::GameUpdate()
{
	HandleInputs();

	ChaseSpaceShip(m_HeavyRock, 0.6f);
	ChaseSpaceShip(m_LightRock, 0.8f);

	PlayerCam = { {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2}, m_SpaceShip.getPos(), 0.0f, 1.0f };
}

static Rectangle GetViewRect(Camera2D camera, float ScreenWidth, float ScreenHeight)
{
	Rectangle viewRect;

	float viewWidth = ScreenWidth / camera.zoom;
	float viewHeight = ScreenHeight / camera.zoom;

	Vector2 origin = {
		camera.target.x - camera.offset.x / camera.zoom - viewWidth / 2,
		camera.target.y - camera.offset.y / camera.zoom - viewHeight / 2
	};

	viewRect = { origin.x, origin.y, viewWidth, viewHeight };

	return viewRect;
}

static void RenderBG(const Rectangle& viewRect, const Texture2D& texture)
{
	int startX = (int)floor(viewRect.x / texture.width);
	int startY = (int)floor(viewRect.y / texture.height);

	for (int x = startX; x <= startX + 1; x++)
	{
		for (int y = startY; y <= startY + 1; y++)
		{
			int posX = x * texture.width;
			int posY = y * texture.height;

			DrawTexture(texture, (int)posX, (int)posY, WHITE);
		}
	}
}

void Game::GameRender() const
{
	Rectangle viewRect = GetViewRect(PlayerCam, SCREEN_WIDTH, SCREEN_HEIGHT);
	RenderBG(viewRect, BackgroundTexture);

	Renderer::RenderSprite(m_HeavyRock);
	Renderer::RenderSprite(m_LightRock);
	Renderer::RenderSprite(m_SpaceShip);
}

void Game::GameShutdown()
{
	UnloadTexture(BackgroundTexture);
	UnloadTexture(m_SpaceShip.getTexture());
	UnloadTexture(m_HeavyRock.getTexture());
	UnloadTexture(m_LightRock.getTexture());

	CloseWindow();
}

void Game::runGame()
{
	GameStartup();

	while (WindowShouldClose() == false)
	{
		GameUpdate();

		BeginDrawing();

		BeginMode2D(PlayerCam);

		ClearBackground(GRAY);
		GameRender();

		EndMode2D();

		EndDrawing();
	}

	GameShutdown();
}