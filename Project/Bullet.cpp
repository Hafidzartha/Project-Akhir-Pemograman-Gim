#include "Bullet.h"

Engine::Bullet::Bullet(Sprite* sprite)
{
	this->sprite = sprite;
}

Engine::Bullet::~Bullet()
{
}

void Engine::Bullet::SetPosition(float x, float y)
{
	sprite->SetPosition(x, y);
}

vec2 Engine::Bullet::GetPosition()
{
	return sprite->GetPosition();
}

void Engine::Bullet::Update(float deltaTime)
{
	float x = GetPosition().x;
	x += xVelocity * deltaTime;
	SetPosition(x, GetPosition().y);
	sprite->Update(deltaTime);
}

void Engine::Bullet::Draw(vec2 cameraOffset)
{
	// Sesuaikan posisi peluru dengan offset kamera
	vec2 adjustedPos = vec2(
		sprite->GetPosition().x - cameraOffset.x,
		sprite->GetPosition().y - cameraOffset.y
	);
	sprite->SetPosition(adjustedPos.x, adjustedPos.y);
	sprite->Draw();

	// Kembalikan posisi asli sprite setelah digambar
	sprite->SetPosition(adjustedPos.x + cameraOffset.x, adjustedPos.y + cameraOffset.y);
}
Engine::Sprite* Engine::Bullet::GetSprite()
{
	return sprite;
}
