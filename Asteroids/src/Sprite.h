#pragma once

#include <raylib.h>
#include <raymath.h>

class Sprite
{
public:
	Sprite(int Health);
	~Sprite();

	Sprite() = delete;
	Sprite(const Sprite& other) = delete;
	Sprite& operator=(const Sprite& other) = delete;

	void setPos(Vector2 Pos);
	void setVelocity(float Velocity);
	void setHealth(int Health);
	void setRect(float x, float y, float w, float h);
	void setRotation(float Rotation);
	void setTexture(const char* path);

	const Vector2& getPos() const;
	float getVelocity() const;
	int getHealth() const;
	const Rectangle& getRect() const;
	float getRotation() const;
	const Texture2D& getTexture() const;

private:
	Vector2 m_SpritePos;
	float m_SpriteVelocity;
	int m_SpriteHealth;
	Rectangle m_SpriteRect;
	float m_Rotation;
	Texture2D m_SpriteTexture;
};