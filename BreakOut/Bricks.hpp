#pragma once

#include "GameObject.hpp"
#include <SDL.h>
#include <iostream>

class Brick : public GameObject
{
private:
	int hp;
	int points;
	SDL_Color color;
	bool destroyed;
	void updateColor()
	{
		switch (hp)   // Changing color based on HP
		{
			case 3:
				color = {200, 0, 50, 255}; // Red for 3 HP
				break;
			case 2:
				color = {250, 100, 0, 255}; // Orange for 2 HP
				break;
			case 1:
				color = {200, 200, 0, 255}; // Yellow for 1 HP
				break;
			default:
				color = {128, 128, 128, 255}; // Grey for destroyed
				break;
		}
	}

public:
	Brick(float x, float y, int w, int h, int hp): GameObject(x, y, w, h), hp(hp), points(hp * 100), destroyed(false)
	{
		updateColor(); // Setting initial color based on hp
	}
	
	void update(float deltaTime) override;
	void render(SDL_Renderer* renderer) override;

	void onCollision(GameObject* other) override
	{
		takeDamage(1);
	}

	void takeDamage(int damage = 1)
	{
		if (destroyed) return;
		hp -= damage;
		if (hp <= 0)
		{
			destroyed = true;
			active = false;
		}
		else
		{
			updateColor();
		}
	}
	bool isDestroyed() const { return destroyed; }
	int getPoints() const { return points; }
	int getHP() const { return hp; }


	~Brick()
	{
		// std::cout << "Brick Destroyed !" << std::endl;
	}
};

void Brick::update(float deltaTime)
{
	if (destroyed)
	{
		active = false;
	}
}

void Brick::render(SDL_Renderer* renderer)
{
	if (!active || destroyed) return;

	SDL_Rect rect = getBounds();

	// Drawing brick body
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &rect);

	// Drawing brick border
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black border
	SDL_RenderDrawRect(renderer, &rect);
}