#include "Sprite.h"

#include <raylib.h>
#include <raymath.h>

#include <iostream>

Sprite::Sprite(int Health)
	:m_SpritePos{ 0.0f, 0.0f },
	m_SpriteVelocity{ 500.0f },
	m_SpriteHealth{ Health },
	m_SpriteRect{ 0.0f, 0.0f, 0.0f, 0.0f },
	m_Rotation{ 0.0f },
	m_SpriteTexture{ NULL }
{

}


Sprite::~Sprite()
{
	if (m_SpriteTexture.id != 0)
	{
		UnloadTexture(m_SpriteTexture);
	}
}

void Sprite::setPos(Vector2 Pos)
{
	m_SpritePos = Pos;
}

void Sprite::setVelocity(float Velocity)
{
	m_SpriteVelocity = Velocity;
}

void Sprite::setHealth(int Health)
{
	m_SpriteHealth = Health;
}

void Sprite::setRect(float x, float y, float w, float h)
{
	m_SpriteRect = { x, y, w, h };
}

void Sprite::setRotation(float Rotation)
{
	m_Rotation = Rotation;
}

void Sprite::setTexture(const char* path)
{
	m_SpriteTexture = LoadTexture(path);
}

const Vector2& Sprite::getPos() const
{
	return m_SpritePos;
}

float Sprite::getVelocity() const
{
	return m_SpriteVelocity;
}

int Sprite::getHealth() const
{
	return m_SpriteHealth;
}

const Rectangle& Sprite::getRect() const
{
	return m_SpriteRect;
}

float Sprite::getRotation() const
{
	return m_Rotation;
}

const Texture2D& Sprite::getTexture() const
{
	return m_SpriteTexture;
}

bool Sprite::isHit(Sprite& sprite)
{
	float radius = std::min(this->getRect().width, this->getRect().height) / 2.0f;
	float otherRadius = std::min(sprite.getRect().width, sprite.getRect().height) / 2.0f;

	Vector2 thisCenter = { this->getPos().x + this->getRect().width / 2.0f, this->getPos().y + this->getRect().height / 2.0f };
	Vector2 otherCenter = { sprite.getPos().x + sprite.getRect().width / 2.0f, sprite.getPos().y + sprite.getRect().height / 2.0f };

	float distance = Vector2Distance(thisCenter, otherCenter);

	return (distance <= (radius + otherRadius));
}