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

void Game::MakeBullets()
{
	m_Bullets.emplace_back(1);

	Sprite& bullet = m_Bullets.back();

	Vector2 spaceshipPos = m_SpaceShip.getPos();
	float spaceshipRotation = m_SpaceShip.getRotation();

	Vector2 bulletDirection = Vector2Rotate({ 0.0f, -1.0f }, spaceshipRotation * DEG2RAD);

	float bulletSpeed = m_SpaceShip.getVelocity() * 2.0f;

	bullet.setTexture("res/laser.png");
	bullet.setPos(spaceshipPos + Vector2Scale(bulletDirection, 20.0f));
	bullet.setRotation(spaceshipRotation);

	bullet.setRect(0.0f, 0.0f, 13.0f, 37.0f);
}

void Game::HandleInputs()
{
	float deltaTime;
	deltaTime = GetFrameTime();

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
		m_SpaceShip.setRotation(currentRotation - 5.0f * (velocity / (2 * PI)) * deltaTime);
	}
	if (IsKeyDown(KEY_D))
	{
		m_SpaceShip.setRotation(currentRotation + 5.0f * (velocity / (2 * PI)) * deltaTime);
	}
	if (IsKeyPressed(KEY_SPACE))
	{
		MakeBullets();
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

void Game::UpdateBullets()
{
	float deltaTime = GetFrameTime();
	Rectangle viewRect = GetViewRect(PlayerCam, float(SCREEN_WIDTH), float(SCREEN_HEIGHT));

	for (size_t i = 0; i < m_Bullets.size();)
	{
		Sprite& bullet = m_Bullets[i];

		Vector2 direction = {
			sinf(DEG2RAD * bullet.getRotation()),
			-cosf(DEG2RAD * bullet.getRotation())
		};

		Vector2 newPos = bullet.getPos() + Vector2Scale(direction, bullet.getVelocity() * deltaTime);
		bullet.setPos(newPos);

		if (newPos.x > 500000 or newPos.y > 500000)
		{
			m_Bullets.erase(m_Bullets.begin() + i);
		}
		else
		{
			++i;
		}
	}
}

void Game::GameUpdate()
{
	HandleInputs();

	UpdateBullets();

	ChaseSpaceShip(m_HeavyRock, 0.6f);
	ChaseSpaceShip(m_LightRock, 0.8f);

	PlayerCam = { {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2}, m_SpaceShip.getPos(), 0.0f, 1.0f };
}

Rectangle Game::GetViewRect(Camera2D camera, float ScreenWidth, float ScreenHeight)
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

	for (const auto& bullet : m_Bullets)
	{
		Renderer::RenderSprite(bullet);
	}

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