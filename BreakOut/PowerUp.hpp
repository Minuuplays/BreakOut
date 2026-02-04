#pragma once

#include <SDL.H>
#include <SDL_image.h>
#include <iostream>
#include "GameObject.hpp"

enum class PowerUpType {
	MultiBall,
	WidePaddle,
	SlowBall,
	ExtraLife,
	LaserPaddle
};

class Laser : public GameObject
{
private:
	float speed;
	SDL_Color color;
public:
	Laser(float x, float y) :GameObject(x, y, 5, 24), speed(300.0f)
	{
		velocity.y = -speed;
		color = { 255, 255, 0, 255 };
	}

	void update(float deltaTime)
	{
		if (!active) return;

		// Move upward
		position += velocity * deltaTime;

		// Deactivate if off screen
		if (position.y < -height) {
			active = false;
		}

	}

	void render(SDL_Renderer* renderer)
	{
		if (!active)
		{
			return;
		}

		SDL_Rect rect = getBounds();

		// Draw laser beam
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		SDL_RenderFillRect(renderer, &rect);

		// White glow effect
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &rect);

		SDL_Rect glowRect = {rect.x - 1, rect.y - 1, rect.w + 2, rect.h + 2};
		SDL_SetRenderDrawColor(renderer, 255, 255, 100, 128);  // Semi-transparent yellow
		SDL_RenderDrawRect(renderer, &glowRect);
	}

	~Laser()
	{
		std::cout << "Laser Destroyed !" << std::endl;
	}
};

class PowerUp : public GameObject {
private:
	PowerUpType type;
	SDL_Texture* texture;
	float fallSpeed;

public:
	PowerUp(float x, float y, PowerUpType t, SDL_Texture* tex): 
		GameObject(x, y, 50, 50), type(t), texture(tex), fallSpeed(100.0f)
	{
		std::cout << "PowerUp Created at (" << x << ", " << y << ")\n";
	}

	// >> Copy Constructor << //
	PowerUp(const PowerUp& other):
		GameObject(other.position.x, other.position.y, other.width, other.height),
		type(other.type), texture(other.texture), fallSpeed(other.fallSpeed)
	{
		this->velocity = other.velocity;
		this->active = other.active;
	}

	PowerUpType getType() const { return type; }

	void update(float deltaTime) override;
	void render(SDL_Renderer* renderer) override;

	~PowerUp()
	{
		// Destructor
	}
};

void PowerUp::update(float deltaTime) 
{
	if (!active) return;

	position.y += fallSpeed * deltaTime; // falling down

	if (position.y > 750) 
	{
		active = false; // Deleting if goes off screen
	}	
}

void PowerUp::render(SDL_Renderer* renderer) 
{
	if (!active) return;

	SDL_Rect rect = getBounds();
	SDL_Rect innerRect = { rect.x + 2, rect.y + 2, rect.w - 4, rect.h - 4 };

	if (texture != nullptr)
	{
		SDL_RenderCopy(renderer, texture, nullptr, &rect);
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White border

	SDL_RenderDrawRect(renderer, &rect);
	SDL_RenderDrawRect(renderer, &innerRect);
}

class PowerUpEffect
{
public:
	bool active;
	PowerUpType type;
	Uint32 startTime;
	Uint32 duration;

	PowerUpEffect(PowerUpType t, Uint32 dur) : active(true), type(t),
		startTime(SDL_GetTicks()), duration(dur) {}

	bool isExpired() const
	{
		return (SDL_GetTicks() - startTime) >= duration;
	}

	float getRemainingTime() const
	{
		Uint32 elapsed = SDL_GetTicks() - startTime;
		
		if (elapsed >= duration) return 0.0f;

		return (duration - elapsed) / 1000.0f;  // return in seconds
	}
};