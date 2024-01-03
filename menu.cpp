/*#include "menu.h"


void renderMenu() {
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, menu, NULL, NULL);
	SDL_RenderCopy(renderer, start, NULL, &start_rect);
	SDL_GetMouseState(&mouse.x, &mouse.y);
	mouse.x *= scaleX;
	mouse.y *= scaleY;

	if (SDL_PointInRect(&mouse, &start_rect)) {
		SDL_SetTextureColorMod(start, 255, 255, 255);
		if (SDL_GetMouseState(&mouse.x, &mouse.y) & SDL_BUTTON(1)) {
			SDL_Delay(100);
			Start = true;
		}
	}
	else SDL_SetTextureColorMod(start, 150, 150, 150);
}

void renderMenuGameOver() {
	replay_rect = { SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2, 100, 100 };
	game_quit_rect = { SCREEN_WIDTH / 2 + 10, SCREEN_HEIGHT / 2, 100, 100 };
	SDL_RenderCopy(renderer, replay, NULL, &replay_rect);
	SDL_RenderCopy(renderer, game_quit, NULL, &game_quit_rect);
	SDL_GetMouseState(&mouse.x, &mouse.y);
	mouse.x *= scaleX;
	mouse.y *= scaleY;

	// if replay button is pressed
	if (SDL_PointInRect(&mouse, &replay_rect)) {
		SDL_SetTextureColorMod(replay, 255, 255, 255);
		if (SDL_GetMouseState(&mouse.x, &mouse.y) & SDL_BUTTON(1)) {
			SDL_Delay(100);
			isChoose = false;
			Start = true;
		}
	}
	else SDL_SetTextureColorMod(replay, 150, 150, 150);

	// if quit button is pressed
	if (SDL_PointInRect(&mouse, &game_quit_rect)) {
		SDL_SetTextureColorMod(game_quit, 255, 255, 255);
		if (SDL_GetMouseState(&mouse.x, &mouse.y) & SDL_BUTTON(1)) {
			SDL_Delay(100);
			quit = true;
		}
	}
	else SDL_SetTextureColorMod(game_quit, 150, 150, 150);

	SDL_RenderPresent(renderer);
}*/