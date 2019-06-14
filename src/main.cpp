#include <iostream>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Piece.h"
#include "Game.h"

int main() {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    TTF_Font *font;
    bool quit = false;

    atexit(SDL_Quit);

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        return 1;
    }

    if (TTF_Init() != 0) {
        return 1;
    }

    window = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SDL_WINDOW_RESIZABLE);
    if (!window) {
        return 2;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        SDL_DestroyWindow(window);
        return 3;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    if (!(font = TTF_OpenFont("../data/ChiKareGo2.ttf", 16))) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        std::cout << "Error loading font: " << TTF_GetError() << std::endl;
        return 4;
    }

    srand(time(nullptr));

    Game game;
    game.setFont(font);

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_z) {
                    game.rotateLeft();
                }
                else if (event.key.keysym.sym == SDLK_x){
                    game.rotateRight();
                }
                else if (event.key.keysym.sym == SDLK_LEFT) {
                    game.left();
                }
                else if (event.key.keysym.sym == SDLK_RIGHT) {
                    game.right();
                }
                else if (event.key.keysym.sym == SDLK_DOWN) {
                    game.move();
                }
                else if (event.key.keysym.sym == SDLK_UP) {
                    game.place();
                }
                else if (event.key.keysym.sym == SDLK_TAB) {
                    game.hold();
                }
                else if (event.key.keysym.sym == SDLK_RETURN) {
                    game.skip();
                }
                else if (event.key.keysym.sym == SDLK_p) {
                    game.pause();
                }
            }
        }

        game.update();

        SDL_RenderClear(renderer);
        game.render(renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(1);
    }

    TTF_CloseFont(font);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    return 0;
}