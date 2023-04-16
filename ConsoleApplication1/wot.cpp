#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Tanks.h"
#include "Map.h"
#include "Menu.h"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

bool game_isPlaying = false;
bool game_wasPaused = false;
bool game_inMenu = true;
bool q = false;

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;
SDL_Rect screenRect = { 0, 0, SCREEN_WIDTH , SCREEN_HEIGHT };
Menu* menu = new Menu();

bool init() {
    bool ok = true;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "Can't init SDL: " << SDL_GetError() << endl;
    }

    win = SDL_CreateWindow("Howis?", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        cout << "Can't create window: " << SDL_GetError() << endl;
        ok = false;
    }

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (ren == NULL) {
        cout << "Can't create renderer: " << SDL_GetError() << endl;
        ok = false;
    }

    return ok;
}

void quit() {
    SDL_DestroyWindow(win);
    win = NULL;

    SDL_DestroyRenderer(ren);
    ren = NULL;

    SDL_Quit();
}

void quit(Tanks* one, Tanks* two){
    one->destroyTex();
    two->destroyTex();

    quit();
}

int main(int arhc, char** argv) {

    if (!init()) {
        quit();
        system("pause");
        return 1;
    }

    SDL_Event e;

    Map* map = new Map("./Maps/map2.txt");
    SDL_Rect tankRect = { 100, 376, 64, 64 };
    SDL_Rect tank2Rect = { 636, 376, 64, 64 };
    Tanks* mainTank = new Tanks(tankRect);
    Tanks* botTank = new Tanks(tank2Rect);
    mainTank->load(ren, "./Images/tank1.bmp");
    botTank->load(ren, "./Images/tank2.bmp");
    mainTank->setDirection('d');
    botTank->setDirection('a');
    

    while (!q) {
        if (game_isPlaying) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_KEYDOWN)
                {
                    if (e.key.keysym.sym == SDLK_p) {
                        game_isPlaying = false;
                        game_wasPaused = true;
                        game_inMenu = false;
                    }

                    // main

                    if (e.key.keysym.sym == SDLK_ESCAPE) {
                        game_inMenu = true;
                        game_isPlaying = false;
                        game_wasPaused = false;
                    }

                    if (e.key.keysym.sym == SDLK_w) {
                        mainTank->moveUp();
                    }

                    else if (e.key.keysym.sym == SDLK_d) {
                        mainTank->moveRight();
                    }

                    else if (e.key.keysym.sym == SDLK_s) {
                        mainTank->moveDown();
                    }

                    else if (e.key.keysym.sym == SDLK_a) {
                        mainTank->moveLeft();
                    }

                    if (e.key.keysym.sym == SDLK_LSHIFT) {
                        mainTank->fire();
                    }

                    // bot

                    if (e.key.keysym.sym == SDLK_UP) {
                        botTank->moveUp();
                    }

                    else  if (e.key.keysym.sym == SDLK_RIGHT) {
                        botTank->moveRight();
                    }

                    else if (e.key.keysym.sym == SDLK_DOWN) {
                        botTank->moveDown();
                    }

                    else if (e.key.keysym.sym == SDLK_LEFT) {
                        botTank->moveLeft();
                    }

                    if (e.key.keysym.sym == SDLK_RSHIFT) {
                        botTank->fire();
                    }
                }

                if (e.type == SDL_KEYUP) {
                    if (e.key.keysym.sym == SDLK_w
                        || e.key.keysym.sym == SDLK_d
                        || e.key.keysym.sym == SDLK_s
                        || e.key.keysym.sym == SDLK_a) {
                        mainTank->stop();
                    }

                    if (e.key.keysym.sym == SDLK_UP
                        || e.key.keysym.sym == SDLK_DOWN
                        || e.key.keysym.sym == SDLK_RIGHT
                        || e.key.keysym.sym == SDLK_LEFT) {
                        botTank->stop();
                    }
                }
            }

            map->draw(ren);
            map->checkCollision(mainTank);
            map->checkCollision(botTank);
            mainTank->handler(ren, botTank, &q);
            botTank->handler(ren, mainTank, &q);
        }

        if (game_inMenu) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    int x = 0, y = 0;
                    SDL_GetMouseState(&x, &y);
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        if (menu->play(x, y)) {
                            game_inMenu = false;
                            game_isPlaying = true;
                        }
                        if (menu->exit(x, y)) {
                            q = true;
                        }
                    }
                }
            }
            menu->draw(ren);
        }

        if (game_wasPaused) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym == SDLK_ESCAPE) {
                        game_inMenu = true;
                        game_isPlaying = false;
                        game_wasPaused = false;
                    }
                    if (e.key.keysym.sym == SDLK_p) {
                        game_wasPaused = false;
                        game_isPlaying = true;
                    }
                }
            }
            SDL_SetRenderDrawColor(ren, 200, 0, 0, 200);
            SDL_RenderClear(ren);
        }

        SDL_RenderPresent(ren);

        SDL_Delay(40);
    }

    SDL_Delay(500);

    quit(mainTank, botTank);
    return 0;
}
