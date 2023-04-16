#pragma once
#include <fstream>

class Map {

	class Block {
	public:
		SDL_Rect rct;
		std::string color;
	};

	static const int size = 20;
	int block = 40;
	Block** rect = new Block*[size];
	char* colors;
	const char* filename;

public:
	Map(const char* filename) {
		this->filename = filename;

		for (int i = 0; i < size; i++) {
			rect[i] = new Block[size];
		}
	}
	
	void draw(SDL_Renderer* ren) {
		std::ifstream file(filename);

		for (int i = 0, offset_y = 0; i < size; i++, offset_y += block) {
			for (int j = 0, offset_x = 0; j < size; j++, offset_x += block) {
				char current;
				file >> current;

				if (current == 'w') {
					rect[i][j].rct = {offset_x, offset_y, block, block};
					rect[i][j].color = "blue";
					SDL_SetRenderDrawColor(ren, 0, 0, 250, 255);
				} else if (current == 's') {
					rect[i][j].rct = { offset_x, offset_y, block, block };
					rect[i][j].color = "white";
					SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
				}
				SDL_RenderFillRect(ren, &rect[i][j].rct);
			}
		}
	}

	void checkCollision(Tanks* tank) {
		SDL_Rect rct = tank->getRect();
		SDL_Rect rctBullet = tank->getBulletRect();
		SDL_Point rectPoints[4] = { {rct.x, rct.y},
			{rct.x + rct.w, rct.y},
			{rct.x + rct.w, rct.y + rct.h},
			{rct.x, rct.y + rct.h} };
		SDL_Point bulletRectPoints[4] = { {rctBullet.x, rctBullet.y},
			{rctBullet.x + rctBullet.w, rctBullet.y},
			{rctBullet.x + rctBullet.w, rctBullet.y + rctBullet.h},
			{rctBullet.x, rctBullet.y + rctBullet.h} };

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				for (int k = 0; k < 4; k++) {
					if (SDL_PointInRect(&rectPoints[k], &rect[i][j].rct)
						&& rect[i][j].color == "blue") {

						if (tank->getDirection() == 'w') {
							tank->stop();
							tank->setPosition(rct.x, rct.y + 3);
						} else if (tank->getDirection() == 's') {
							tank->stop();
							tank->setPosition(rct.x, rct.y - 3);
						} else if (tank->getDirection() == 'd') {
							tank->stop();
							tank->setPosition(rct.x - 3, rct.y);
						} else if (tank->getDirection() == 'a') {
							tank->stop();
							tank->setPosition(rct.x + 3, rct.y);
						}
					}

					if (SDL_PointInRect(&bulletRectPoints[k], &rect[i][j].rct)
						&& rect[i][j].color == "blue") {

						tank->finishFiring();
					}
				}
			}
		}
	}
};