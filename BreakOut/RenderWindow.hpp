#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class RenderWindow
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
public:
    RenderWindow(const char* p_title, int p_w, int p_h) : window(nullptr), renderer(nullptr)
    {
        window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            p_w, p_h, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            throw std::runtime_error(std::string("Window creation failed: ") + SDL_GetError());
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr) {
            SDL_DestroyWindow(window);
            throw std::runtime_error(std::string("Renderer creation failed: ") + SDL_GetError());
        }
    }

    SDL_Texture* loadTexture(const char* p_filePath);

	void cleanupTexture(SDL_Texture* texture) // >> Internal function << //
    {
        if (texture != nullptr) {
            SDL_DestroyTexture(texture);
        }
    }

    void clear()
    {
        SDL_RenderClear(renderer);
	}

    void render(SDL_Texture* p_texture)
    {
        SDL_RenderCopy(renderer, p_texture, nullptr, nullptr);
	}

    void display()
    {
        SDL_RenderPresent(renderer);
    }

    // >> Inline function << //
    inline SDL_Renderer* getRenderer() const { return renderer; }

    ~RenderWindow()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }
};

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath)  // >> External function << //
{
    SDL_Texture* texture = nullptr;
    texture = IMG_LoadTexture(renderer, p_filePath);
    if (texture == nullptr) {
        std::cerr << "Texture loading error: " << SDL_GetError() << std::endl;
    }
    return texture;
}


