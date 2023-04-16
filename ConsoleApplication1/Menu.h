#pragma once

class Menu {
	SDL_Rect toPlay = {250, 250, 300, 100};
	SDL_Rect toExit = { 250, 400, 300, 100};

public:
	void draw(SDL_Renderer* ren) {
		SDL_SetRenderDrawColor(ren, 100, 100, 100, 255);
		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren, 0, 100, 0, 255);
		SDL_RenderFillRect(ren, &toPlay);

		SDL_SetRenderDrawColor(ren, 100, 0, 0, 255);
		SDL_RenderFillRect(ren, &toExit);
	}

	void clicked(int x, int y, bool play, bool quit, bool menu) {
		SDL_Point click = { x, y };

		if (SDL_PointInRect(&click, &toPlay)) {
			play = true;
			menu = false;
		}
		if (SDL_PointInRect(&click, &toExit)) {
			quit = true;
		}
	}

	bool play(int x, int y) {
		SDL_Point click = { x, y };

		if (SDL_PointInRect(&click, &toPlay)) {
			return true;
		}

		return false;
	}

	bool exit(int x, int y) {
		SDL_Point click = { x, y };

		if (SDL_PointInRect(&click, &toExit)) {
			return true;
		}

		return false;
	}
};