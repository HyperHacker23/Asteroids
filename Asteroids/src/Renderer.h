#pragma once

#include "Sprite.h"

#include <raylib.h>

class Renderer
{
public:
	Renderer() = default;
	~Renderer() = default;

	static void RenderSprite(const Sprite& sprite);
};