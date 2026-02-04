#pragma once

#include "templates.hpp"
#include "Vector2D.hpp"
#include "GameObject.hpp"
#include <SDL.h>
#include <iostream>
#include <cmath>

constexpr float PI = 3.14159265f;

class Ball : public GameObject
{
private:
	float radius;
	float speed;
	int screenWidth;
	int screenHeight;
	SDL_Color color;
	bool launched;

public:
	// >> Constructor  Overloading << //
	Ball(float x, float y, float r, int scW, int scH)
		: GameObject(x, y, static_cast<int>(r * 2), static_cast<int>(r * 2)),
		radius(r), speed(300.0f), screenWidth(scW), screenHeight(scH), launched(false)
	{
		activeCount++;
		color = { 100, 250, 200, 0 };
		speed = 350.0f;
		velocity = Vector2D(0, 0); // Ball starts stationary

		std::cout << "Ball created at (" << x << ", " << y << ") with radius " << r << std::endl;

	}
	Ball(float x, float y, float r, int scW, int scH, float initSpeed)
		: GameObject(x, y, static_cast<int>(r * 2), static_cast<int>(r * 2)),
		radius(r), speed(initSpeed), screenWidth(scW), screenHeight(scH), launched(false)
	{
		activeCount++;
		color = { 222, 222, 222, 222 };

		velocity = Vector2D(0, 0); // Ball starts stationary

		std::cout << "Ball created with custom Speed:" << initSpeed << std::endl;

	}

	static int activeCount;

	float getRadius() const { return radius; }
	bool isLaunched() const { return launched; }
	bool setLaunched(bool state) { return launched = state; }

	void update(float deltaTime) override;
	void render(SDL_Renderer* renderer) override;

	void launch(float angleinDegrees)
	{
		float angleinRadians = angleinDegrees * (PI / 180.0f);
		
		// Setting Velocity based on angle
		velocity.x = speed * cos(angleinRadians);
		velocity.y = -speed * sin(angleinRadians); // Negative because in y axis goes downwards
       
		float currentSpeed = velocity.length();
		currentSpeed = clamp(currentSpeed, 200.0f, 800.0f);

		Vector2D normalized = velocity.normalize();
		velocity = normalized * currentSpeed;

		launched = true;

		std::cout << "Ball launched at angle: " << angleinDegrees <<
			" degrees with velocity" << velocity.x << ", " << velocity.y << std::endl;
	}
	void revX() { velocity.x = -velocity.x; }
	void revY() { velocity.y = -velocity.y; }
	void reset(float x, float y)
	{
		position.x = x;
		position.y = y;
		velocity = Vector2D(0, 0);
		launched = false;
		active = true;
	}

	~Ball() 
	{
		activeCount--;
		std::cout << "Ball destroyed" << std::endl;
	}
};

int Ball::activeCount = 0;

// Ball Movement and Collision with Walls
void Ball::update(float deltaTime)
{
	if (!active || !launched) return; // Skip if not active or launched

	position += velocity * deltaTime; // operator overloading

	if (position.x - radius < 0) // Bounce on Left Wall
	{
		position.x = radius;
		revX();
	}
	if (position.x + radius >=screenWidth ) // Bounce on Right Wall
	{
		position.x = screenWidth - radius;
		revX();
	}
	if (position.y - radius <= 0) // Bounce on Top Wall
	{
		position.y = radius;
		revY();
	}
	if (position.y - radius > screenHeight) // Ball goes offScreen
	{
		launched = false;
		active = false;
		std::cout << "Ball went off screen and is reset." << std::endl;
	}
}

// Ball Rendering
void Ball::render(SDL_Renderer* renderer)
{
	if (!active) return; // Skip rendering if not active

	int centerX = static_cast<int>(position.x);
	int centerY = static_cast<int>(position.y);
	int r = static_cast<int>(radius);

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

	// Drawing circle using Midpoint Circle Algorithm
	for (int w = 0; w < r * 2; w++)
	{
		for (int h = 0; h < r * 2; h++)
		{
			int dx = r - w; // horizontal offset
			int dy = r - h; // vertical offset
			if ((dx * dx + dy * dy) <= (r * r ))
			{
				SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
			}
		}
	}
}