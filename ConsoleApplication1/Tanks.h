#pragma once

#include <SDL.h>
#include <iostream>
#include <string>

class Tanks {
	SDL_Texture* texture;
	SDL_Rect rect;
	int speed;
	int hp;

	bool isReloading = false;
	int kdReload;

	bool movingUp = false;
	bool movingDown = false;
	bool movingLeft = false; 
	bool movingRight = false;
	bool firing = false;
	bool taran = false;

	char direction;
	char fireDirection;

	SDL_Rect rectBullet;
	int bulletSpeed = 20;
	int distanceCount = 0;
	int fireDistance = 20;

public:
	Tanks(SDL_Rect rct) {
		texture = NULL;
		rect = rct;
		speed = 10;
		rectBullet = { this->rect.x + this->rect.w / 2, this->rect.y + this->rect.h / 2, 12, 12 };
		hp = 100;
	}

	void setDirection(char dir) {
		this->direction = dir;
	}

	char getFireDirection() {
		return this->fireDirection;
	}

	SDL_Rect getBulletRect() {
		return this->rectBullet;
	}

	char getDirection() {
		return this->direction;
	}

	void setPosition(int x, int y) {
		this->rect.x = x;
		this->rect.y = y;
	}

	void handler(SDL_Renderer* ren, Tanks* other, bool q) {
		if (hp > 0) {
			SDL_RenderCopy(ren, texture, NULL, &rect);

			if (movingUp) {
				this->rect.y -= speed;
				this->direction = 'w';
				this->load(ren, "./Images/up_tank1.bmp");
				taran = true;
			}

			if (movingDown) {
				this->rect.y += speed;
				this->direction = 's';
				this->load(ren, "./Images/d_tank1.bmp");
				taran = true;
			}

			if (movingRight) {
				this->rect.x += speed;
				this->direction = 'd';
				this->load(ren, "./Images/tank1.bmp");
				taran = true;
			}

			if (movingLeft) {
				this->rect.x -= speed;
				this->direction = 'a';
				this->load(ren, "./Images/l_tank1.bmp");
				taran = true;
			}

			if (isReloading) {
				kdReload--;
				if (kdReload == 0) {
					isReloading = false;
				}
			}
			else {
				if (firing) {
					if (this->distanceCount == this->fireDistance) {
						finishFiring();
					}
					if (checkCollision(rectBullet, other->rect)) {
						other->hp -= 50;
					}

					this->shoting(this->fireDirection);
					SDL_SetRenderDrawColor(ren, 255, 0, 3, 0xFF);
					SDL_RenderFillRect(ren, &rectBullet);
					this->distanceCount++;
				}
			}

			if (checkCollision(this->rect, other->rect) && taran) {
				other->hp -= 2;
			}
		}
		else {
			death(ren);
		}
	}

	void reloading() {
		kdReload = 10;
		isReloading = true;
	}

	void finishFiring() {
		this->distanceCount = 0;
		this->firing = false;
	}

	void death(SDL_Renderer* ren) {
		hp = 0;
		SDL_DestroyTexture(texture);
		texture = NULL;
		rect.x = -10;
		rect.y = -10;
	}

	void destroyTex() {
		SDL_DestroyTexture(texture);
	}

	void fire() {
		if (firing) return;
		else {
			this->firing = true;
			fireDirection = direction;
			rectBullet.x = this->rect.x + this->rect.w / 2;
			rectBullet.y = this->rect.y + this->rect.h / 2;
			reloading();
		}
	}

	void shoting(const char dir) {
		if (dir == 'a') {
			this->rectBullet.x -= this->bulletSpeed;
		}

		if (dir == 'd') {
			this->rectBullet.x += this->bulletSpeed;
		}

		if (dir == 'w') {
			this->rectBullet.y -= this->bulletSpeed;
		}

		if (dir == 's') {
			this->rectBullet.y += this->bulletSpeed;
		}
	}

	SDL_Texture* getTexture() {
		return this->texture;
	}
	
	SDL_Rect getRect() {
		return this->rect;
	}

	void load(SDL_Renderer* ren, const char* filename) {
		SDL_Surface* tmp = NULL;
		tmp = IMG_Load(filename);

		if (tmp == NULL) {
			std::cout << "IMG " << filename << "hasn't loaded: " << SDL_GetError() << std::endl;
		}

		this->texture = SDL_CreateTextureFromSurface(ren, tmp);

		if (this->texture == NULL) {
			std::cout << "Texture " << filename << "hasn't created: " << SDL_GetError() << std::endl;
		}

		SDL_FreeSurface(tmp);
	}

	void moveUp() {
		movingUp = true;
		movingDown = false;
		movingLeft = false;
		movingRight = false;
	}

	void moveDown() {
		movingDown = true;
		movingUp = false;
		movingLeft = false;
		movingRight = false;
	}

	void moveRight() {
		movingRight = true;
		movingUp = false;
		movingDown = false;
		movingLeft = false;
	}

	void moveLeft() {
		movingLeft = true;
		movingUp = false;
		movingDown = false;
		movingRight = false;
	}

	void stop() {
		movingUp = false;
		movingDown = false;
		movingLeft = false;
		movingRight = false;
		taran = false;
	}

	bool checkCollision(SDL_Rect one, SDL_Rect two) {
		SDL_Point rectPoints[4] = { {one.x, one.y},
			{one.x + one.w, one.y},
			{one.x + one.w, one.y + one.h},
			{one.x, one.y + one.h} };

		for (int i = 0; i < 4; i++) {
			if (SDL_PointInRect(&rectPoints[i], &two)) {
				return true;
			}
		}
		return false;
	}
};
