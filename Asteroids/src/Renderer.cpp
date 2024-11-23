#include "Renderer.h"
#include "Sprite.h"

#include <raylib.h>

void Renderer::RenderSprite(const Sprite& sprite)
{
	Rectangle dest = {
		sprite.getPos().x,
		sprite.getPos().y,
		sprite.getRect().width,
		sprite.getRect().height
	};

	DrawTexturePro(
		sprite.getTexture(),
		sprite.getRect(),
		dest,
		{ sprite.getRect().width / 2, sprite.getRect().height / 2 },
		sprite.getRotation(),
		WHITE
	);
}