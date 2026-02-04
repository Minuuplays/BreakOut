#pragma once
#include "GameObject.hpp"
#include <SDL.h>
#include <iostream>
#include "templates.hpp"

class Paddle : public GameObject
{
private:
	float speed;
	int screenWidth;
	SDL_Color color;
	int originWidth;
	bool hasLaser;

public:
	Paddle(float x, float y, int w, int h, int scW) : GameObject(x, y, w, h), 
		speed(400.0f), screenWidth(scW), hasLaser(false), originWidth(w)
	{
		color = { 0, 0, 0, 0 };
		std::cout << "Paddle created at (" << x << ", " << y << ")" << std::endl;
	}

	void setSpeed(float spd) { speed = spd; }
	float getSpeed() const { return speed; }

	void setWidth(int w) { width = w; }
	int getWidth() const { return width; }
	void resetWidth() { width = originWidth; } 

	void enableLaser() { hasLaser = true; }
	void disableLaser() { hasLaser = false; }
	bool canShoot() const { return hasLaser; }

	void update(float deltaTiem) override;
	void render(SDL_Renderer* renderer) override;

	void handleInput(const Uint8* keyState)
	{
		velocity.x = 0; // Reseting velocity each frame
		
		if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A])
		{
			velocity.x = -speed; 
		}

		if (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D])
		{
			velocity.x = speed;
		}

		position.x += velocity.x * 0.016f; // Operator Overloding
	}

	~Paddle()
	{
		std::cout << "Paddle destroyed" << std::endl;
	}
};

void Paddle::update(float deltaTime)
{
	const Uint8* keyState = SDL_GetKeyboardState(nullptr);
	handleInput(keyState);

	// Keeping paddle within screenFrame
	position.x = clamp(position.x, 0.0f, float(screenWidth - width));
}

void Paddle::render(SDL_Renderer* renderer)
{
	if (!active) return; // Skip rendering if not active
	SDL_Rect rect = getBounds(); // Get paddle bounds
	SDL_Rect rect2 = { rect.x - 1, rect.y - 1, rect.w + 2, rect.h + 2 };

	if (hasLaser) {
		SDL_SetRenderDrawColor(renderer, 255, 150, 155, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 77, 0, 42, 255);
	}
	
	SDL_RenderFillRect(renderer, &rect); // Render paddle

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &rect);
	SDL_RenderDrawRect(renderer, &rect2);
}