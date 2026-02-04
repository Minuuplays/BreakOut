#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>

using namespace std;

enum class GameState {
	MENU,
	PLAYING,
	PAUSED,
	GAMEOVER,
	WIN
};

class GameUI {
private: 
	TTF_Font* font;
	TTF_Font* titleFont;
	SDL_Renderer* renderer;
	void overlay()
	{
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100); // Black with alpha
		SDL_Rect overlay = { 0, 0, 1280, 720 };
		SDL_RenderFillRect(renderer, &overlay);
	}

public:
	GameUI(SDL_Renderer* ren) : renderer(ren), font(nullptr), titleFont(nullptr)
	{
		if (TTF_Init() == -1) {
			throw runtime_error(string("TTF_Init Error: ") + TTF_GetError());
		}

		// Loading Fonts
		font = TTF_OpenFont("Assets/calibrib.ttf", 24);
		titleFont = TTF_OpenFont("Assets/calibrib.ttf", 48);

		if (!font || !titleFont) {
			throw runtime_error(string("TTF_OpenFont Error: ") + TTF_GetError());
		}

		cout << "GameUI initialized successfully!" << endl;
	}

	// Text Rendering Function
	void renderText(const string& text, int x, int y, SDL_Color color, bool centered = false)
	{
		if (!font) return;

		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
		if (!surface) return;

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (!texture) {
			SDL_FreeSurface(surface);
			return;
		}

		SDL_Rect rect{};
		rect.w = surface->w;
		rect.h = surface->h;
		rect.x = centered ? (x - rect.w/ 2) : x;
		rect.y = y;

		SDL_RenderCopy(renderer, texture, nullptr, &rect);

		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surface);	
	}
	void renderLargeText(const string& text, int x, int y, SDL_Color color, bool centered = false)
	{
		if (!titleFont) return;

		SDL_Surface* surface = TTF_RenderText_Solid(titleFont, text.c_str(), color);
		if (!surface) return;

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (!texture) {
			SDL_FreeSurface(surface);
			return;
		}

		SDL_Rect rect{};
		rect.w = surface->w;
		rect.h = surface->h;
		rect.x = centered ? (x - rect.w/ 2) : x;
		rect.y = y;

		SDL_RenderCopy(renderer, texture, nullptr, &rect);

		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surface);
	}
	
	void renderButton(const string& text, int x, int y, int w, int h, bool highlighted)
	{
		SDL_Rect rect = { x, y, w, h };

		// Button Background
		if (highlighted) {
			SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255); // Highlighted color
		} else {
			SDL_SetRenderDrawColor(renderer, 50, 50, 100, 255); // Normal color
		}
		SDL_RenderFillRect(renderer, &rect);

		// Button Border
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White border
		SDL_RenderDrawRect(renderer, &rect);

		// Button Text
		SDL_Color textColor = { 255, 255, 255, 255 }; // White text
		renderText(text, x + w / 2, y + (h - 24) / 2, textColor, true);
	}

	void renderMenu(int selected)
	{
		SDL_Color white = { 255, 255, 255, 255 };
		SDL_Color yellow = { 255, 255, 0, 255 };

		// Game Title
		renderLargeText("BREAKOUT", 640, 150, yellow, true);

		// Menu Options
		int buttonY = 300;
		int buttonSpacing = 80;

		renderButton("Start Game", 440, buttonY, 400, 60, selected == 0);
		renderButton("Quit", 440, buttonY + buttonSpacing, 400, 60, selected == 1);

		// Instructions
		renderText("Use Arrow Keys to Navigate, Enter to Select", 640, 620, white, true);
	}

	void renderPauseScreen()
	{
		SDL_Color white = { 255, 255, 255, 255 };
		SDL_Color yellow = { 255, 255, 0, 255 };

		overlay();

		// Pause Text
		renderLargeText("PAUSED", 640, 250, yellow, true);
		renderText("Press SPACE to Resume", 640, 350, white, true);
		renderText("Press ESC to Quit to Menu", 640, 400, white, true);
	}

	void renderGameOverScreen(int score, bool isNewHighScore = false)
	{
		SDL_Color white = { 255, 255, 255, 255 };
		SDL_Color red = { 255, 0, 0, 255 };
		SDL_Color gold = { 255, 215, 0, 255 };
		
		overlay();

		// Game Over Text
		renderLargeText("GAME OVER", 640, 240, red, true);
		if (isNewHighScore) {
			renderText("*** NEW HIGH SCORE! ***", 640, 320, gold, true);
		}
		renderText("Final Score: " + to_string(score), 640, 400, white, true);
		renderText("Press ENTER to Return to Menu", 640, 450, white, true);
		renderText("Press ESC to Quit", 640, 500, white, true);
	}

	void renderWinScreen(int score)
	{
		SDL_Color white = { 255, 255, 255, 255 };
		SDL_Color green = { 0, 255, 0, 255 };
		
		overlay();

		// Win Text
		renderLargeText("YOU WIN!", 640, 200, green, true);
		//renderLargeText("CONGRATULATIONS!", 640, 300, green, true);
		renderText("Your Score: " + to_string(score), 640, 370, white, true);
		renderText("Press ENTER to go to Next Level", 640, 460, white, true);
		renderText("Press ESC to Quit", 640, 550, white, true);
	}

	void renderGameWinScreen(int score)
	{
		SDL_Color white = { 255, 255, 255, 255 };
		SDL_Color green = { 0, 255, 0, 255 };

		overlay();

		// Win Text
		renderLargeText("YOU WIN!", 640, 200, green, true);
		renderLargeText("CONGRATULATIONS!You've completed all levels!", 640, 300, green, true);
		renderText("Final Score: " + to_string(score), 640, 370, white, true);
		renderText("Press ENTER to go to Menu", 640, 460, white, true);
		renderText("Press ESC to Quit", 640, 550, white, true);
	}

	void renderHUD(int score, int lives, int level, int highscore)
	{
		SDL_Color white = { 255, 255, 255, 255 };
		SDL_Color yellow = { 255, 255, 0, 255 };
		SDL_Color gold = { 255, 215, 0, 255 };

		renderText("Score: " + to_string(score), 20, 20, yellow);   // top-left
		renderText("Best: " + to_string(highscore), 220, 20, gold); // top-left below score
		renderText("Lives: " + to_string(lives), 580, 20, white);   // top-center
		renderText("Level: " + to_string(level), 1150, 20, yellow); // top-right
	}


	~GameUI()
	{
		if (font) TTF_CloseFont(font);
		if (titleFont) TTF_CloseFont(titleFont);
		TTF_Quit();
	}

};
