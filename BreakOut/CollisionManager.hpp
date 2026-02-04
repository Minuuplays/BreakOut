#pragma once

#include "Vector2D.hpp"
#include "GameObject.hpp"
#include "Paddle.hpp"
#include "Ball.hpp"
#include "Bricks.hpp"
#include <iostream>
#include <SDL.H>
#include <cmath>

class CollisionManager
{
private:
	// Horizontal Overlap
	static float getOverlapX(const SDL_Rect& a, const SDL_Rect& b)
	{
		float leftOverlap = (a.x + a.w) - b.x;
		float rightOverlap = (b.x + b.w) - a.x;
		return getMin(leftOverlap, rightOverlap);
	}
	// Vertical Overlap
	static float getOverlapY(const SDL_Rect& a, const SDL_Rect& b)
	{
		float topOverlap = (a.y + a.h) - b.y;
		float bottomOverlap = (b.y + b.h) - a.y;
		return getMin(topOverlap, bottomOverlap);
	}

public:
	// check AABB - Axis Aligned Bounding Box
	static bool checkAABB(const SDL_Rect& a, const SDL_Rect& b)
	{
		return (a.x < b.x + b.w &&
			    a.x + a.w > b.x &&
			    a.y < b.y + b.h &&
			    a.y + a.h > b.y );
	}
	// check Ball Paddle Collision
	static bool checkBallPaddle(Ball* ball, Paddle* paddle)
	{
		if (!ball->isActive() || !paddle->isActive()) return false;
		if (!ball->isLaunched()) return false;

		SDL_Rect ballRect = ball->getBounds();
		SDL_Rect paddleRect = paddle->getBounds();

		if (checkAABB(ballRect, paddleRect))
		{
			// Calculating hit position on paddle
			float ballCenterX = ball->getPosition().x;
			float paddleCenterX = paddle->getPosition().x + paddle->getWidth() / 2.0f;
			float hitPos = (ballCenterX - paddleCenterX) / (paddle->getWidth() / 2.0f); // Normalized hit position (-1 to 1)
			
			// Clamping hitPos to [-1, 1]
			if (hitPos < -1.0f) hitPos = -1.0f;
			if (hitPos > 1.0f) hitPos = 1.0f;

			float bounceAngle = hitPos * 60.0f; // Max bounce angle of 60 degrees

			float currentSpeed = ball->getVelocity().length();

			// Calculating new velocity based on bounce angle
			float angleInRadians = ( bounceAngle )* (3.14159265f / 180.0f);
			Vector2D newVelocity(
				currentSpeed * std::sin(angleInRadians),
				-currentSpeed * std::cos(angleInRadians) // Negative because y axis goes downwards
			);
			
			ball->setVelocity(newVelocity);

			// positioning ball above paddle to prevent multiple collisions
			Vector2D ballPos = ball->getPosition();
			ballPos.y = paddle->getPosition().y - ball->getRadius() - 1;
			ball->setPosition(ballPos);

			return true;
		}
		return false;
	}
	// check Ball Brick Collision
	static bool checkBallBrick(Ball* ball, Brick* brick)
	{
		if (!ball->isActive() || !brick->isActive()) return false;
		if (brick->isDestroyed()) return false;
		if (!ball->isLaunched()) return false;

		SDL_Rect ballRect = ball->getBounds();
		SDL_Rect brickRect = brick->getBounds();

		if (checkAABB(ballRect, brickRect))
		{
			// Determining collision side
			float overlapX = getOverlapX(ballRect, brickRect);
			float overlapY = getOverlapY(ballRect, brickRect);

			if (overlapX < overlapY) { ball->revX(); }
			else { ball->revY(); }

			brick->onCollision(ball);

			return true;
		}
		return false;
	}

	// >> Friend Function << //
	friend bool checkCollision(GameObject* a, GameObject* b);
};

bool checkCollision(GameObject* a, GameObject* b)
{
	if (!a->isActive() || !b->isActive()) return false;

	SDL_Rect rectA = a->getBounds();
	SDL_Rect rectB = b->getBounds();

	return CollisionManager::checkAABB(rectA, rectB);
}