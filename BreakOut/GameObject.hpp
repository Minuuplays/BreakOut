#pragma once
#include <SDL.h>
#include "Vector2D.hpp"

// >> Abstract Class << //
class GameObject
{
protected:
	Vector2D position;
	Vector2D velocity;
	int width;
	int height;
	bool active;

public:
	GameObject(float x,float y, int w,int h)
		: position(x, y), velocity(0, 0), width(w), height(h), active(true) {
	}

	// >> Getter and Setter << //
	Vector2D getPosition() const { return position; }
	Vector2D getVelocity() const { return velocity; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	bool isActive() const { return active; }

	void setPosition(const Vector2D& position) { this->position = position; }
	void setVelocity(const Vector2D& velocity) { this->velocity = velocity; }
	void setActive(bool state) { this->active = state; }

	SDL_Rect getBounds() const 
	{
		SDL_Rect bounds{};
		bounds.x = static_cast<int>(position.x); // >> static_cast for type conversion << //
		bounds.y = static_cast<int>(position.y);
		bounds.w = width;	
		bounds.h = height;
		return bounds;
	}

	// >> Pure virtual function << //
	virtual void update(float deltaTime) = 0;
	virtual void render(SDL_Renderer* renderer) = 0;

	// >> Virtual Function << //
	virtual void onCollision(GameObject* other) {}
	virtual ~GameObject() {}
};